from flask import Flask, request, jsonify
from flask_sqlalchemy import SQLAlchemy
import xml.etree.ElementTree as ET
import time
from sqlalchemy import text
import logging

# ---------- XML AYARLARINI YÜKLE ----------
def load_db_config_from_xml(file_path):
    tree = ET.parse(file_path)
    root = tree.getroot()
    conn = root.find("baglanti")
    return {
        "server": conn.attrib["veri_kaynagi"],
        "database": conn.attrib["veritabani"],
        "username": conn.attrib["kullanici"],
        "password": conn.attrib["sifre"]
    }

# ---------- FLASK UYGULAMASI ----------
app = Flask(__name__)

# XML'den config oku
config = load_db_config_from_xml("ayarlar.xml")
print("👉 Server:", config["server"])


# ❗ quote_plus kullanmıyoruz artık!
DB_CONNECTION_STRING = (
    f"mssql+pyodbc://{config['username']}:{config['password']}@{config['server']}/"
    f"{config['database']}?driver=ODBC+Driver+17+for+SQL+Server"
)
print("👉 Connection string:", DB_CONNECTION_STRING)
app.config['SQLALCHEMY_DATABASE_URI'] = DB_CONNECTION_STRING
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)

# ---------- MODELLER ----------
class SensorData(db.Model):
    id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    device_id = db.Column(db.String(50), nullable=False)
    temperature = db.Column(db.Float, nullable=False)
    humidity = db.Column(db.Float, nullable=False)
    timestamp = db.Column(db.Integer, nullable=False)

class Order(db.Model):
    __tablename__ = 'tblMusteriSiparisleri'  # ← bu tablo adını senin veritabanına göre ayarla
    id = db.Column("MusteriSiparisID", db.Integer, primary_key=True)
    code = db.Column("MusteriOrderNo", db.String)
    bant_id = db.Column("bant_id", db.Integer)

class OperationRecord(db.Model):
    __tablename__ = "tblSayacHamVeri"
    id            = db.Column(db.Integer, primary_key=True, autoincrement=True)
    user_id       = db.Column(db.Integer, nullable=False)
    machine_id    = db.Column(db.String(50), nullable=False)
    operation_id  = db.Column(db.Integer, nullable=False)
    timestamp     = db.Column(db.Integer, nullable=False)
    counter       = db.Column(db.Integer, nullable=False)

# ---------- ENDPOINTS ----------
@app.route("/post-data", methods=["POST"])
def post_data():
    # Detailed logging of incoming request
    app.logger.info(f"Incoming request headers: {request.headers}")
    app.logger.info(f"Incoming request content type: {request.content_type}")
    
    # Raw request data logging
    raw_data = request.get_data(as_text=True)
    app.logger.info(f"Raw request data: {raw_data}")

    try:
        # Attempt to parse JSON with different methods
        if request.is_json:
            # Try standard JSON parsing first
            data = request.get_json()
        else:
            # If not JSON, try parsing raw data
            try:
                data = json.loads(raw_data)
            except json.JSONDecodeError:
                app.logger.error("Failed to parse JSON from request")
                return jsonify({"error": "Invalid JSON payload"}), 400

        # Validate all required fields
        required_fields = ["user_id", "machine_id", "operation_id", "counter"]
        for field in required_fields:
            if field not in data:
                app.logger.error(f"Missing required field: {field}")
                return jsonify({"error": f"Missing field: {field}"}), 400

        # Type checking and conversion
        try:
            user_id = int(data["user_id"])
            machine_id = str(data["machine_id"])
            operation_id = int(data["operation_id"])
            counter = int(data["counter"])
        except (ValueError, TypeError) as e:
            app.logger.error(f"Type conversion error: {e}")
            return jsonify({"error": "Invalid data types"}), 400

        # Timestamp handling
        ts = int(data.get("timestamp", time.time()))

        # Create and save record
        rec = OperationRecord(
            user_id=user_id,
            machine_id=machine_id,
            operation_id=operation_id,
            timestamp=ts,
            counter=counter
        )
        db.session.add(rec)
        db.session.commit()

        return jsonify({"message": "Record saved successfully"}), 200

    except Exception as e:
        # Comprehensive error logging
        db.session.rollback()
        app.logger.error(f"Unexpected error: {str(e)}")
        app.logger.error(f"Full error details: {e}", exc_info=True)
        return jsonify({"error": "Internal server error", "details": str(e)}), 500

# Configure logging
logging.basicConfig(level=logging.DEBUG)



@app.route("/users", methods=["GET"])
def get_users():
    try:
        result = db.session.execute(text("""
            SELECT UserID, UserName AS username, UserPassword AS hashed_password, Aktif AS aktif
            FROM tblUser WHERE Aktif = 1
        """))
        users = []
        for row in result:
            users.append({
                "id": row.UserID,
                "username": row.username,
                "hashed_password": row.hashed_password,
                "aktif": bool(row.aktif)
            })
        return jsonify(users), 200
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route("/operations", methods=["GET"])
def get_operations_by_order():
    order_id = request.args.get("order_id", type=int)
    if order_id is None:
        return jsonify({"error": "order_id parametresi gerekli"}), 400

    try:
        result = db.session.execute(text("""
            SELECT verimlilik_operasyon_adi, verimlilik_operasyon_birim_sure, verimlilik_operasyon_kesim_sayisi, verimlilik_operasyon_birim_sure_tolerans
            FROM v_operasyonlar
            WHERE MusteriSiparisID = :order_id
        """), {"order_id": order_id})

        operations = []
        for row in result:
            operations.append({
                "name": row.verimlilik_operasyon_adi,
                "unit_time": row.verimlilik_operasyon_birim_sure,
                "cut_count": row.verimlilik_operasyon_kesim_sayisi,
                "unit_time_tol": row.verimlilik_operasyon_birim_sure_tolerans,
            })
        return jsonify(operations), 200
    except Exception as e:
        return jsonify({"error": str(e)}), 500



@app.route("/get-bantlar", methods=["GET"])
def get_bantlar():
    try:
        result = db.session.execute(text("SELECT bant_id, bant_tanim FROM v_bantlar"))
        bantlar = [
            {
                "id": row.bant_id, 
                "tanım": row.bant_tanim
            }
            for row in result
        ]
        return jsonify(bantlar), 200
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route("/orders", methods=["GET"])
def get_orders_by_bant():
    bant_id = request.args.get("bant_id", type=int)
    if bant_id is None:
        return jsonify({"error": "bant_id parametresi gerekli"}), 400

    try:
        result = Order.query.filter_by(bant_id=bant_id).all()
        data = [
            {
                "id": order.id,
                "code": order.code,
                "bant_id": order.bant_id
            }
            for order in result
        ]
        return jsonify(data), 200
    except Exception as e:
        return jsonify({"error": str(e)}), 500


# ---------- BAŞLAT ----------
if __name__ == "__main__":
    print("🟡 Uygulama başlatılıyor...")
    with app.app_context():
        try:
            db.session.execute(text("SELECT 1"))  # Test
            print("✅ Veritabanı bağlantısı başarılı.")
        except Exception as e:
            print("❌ Veritabanına bağlanırken hata oluştu:", e)

    app.run(host="0.0.0.0", port=5000, debug=True)
