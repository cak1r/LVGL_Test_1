from flask import Flask, request, jsonify
from flask_sqlalchemy import SQLAlchemy
import xml.etree.ElementTree as ET
import time
from sqlalchemy import text

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
    id = db.Column("OrderNo", db.Integer, primary_key=True)
    code = db.Column("MusteriOrderNo", db.String)
    bant_id = db.Column("bant_id", db.Integer)

# ---------- ENDPOINTS ----------
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

@app.route("/get-bantlar", methods=["GET"])
def get_bantlar():
    try:
        result = db.session.execute(text("SELECT bant_id, bant_tanim FROM v_bantlar"))
        bantlar = [{"id": row.bant_id, "tanım": row.bant_tanim} for row in result]
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

@app.route("/post-data", methods=["POST"])
def post_data():
    try:
        data = request.get_json()
        new_entry = SensorData(
            device_id=data.get("device_id"),
            temperature=data.get("temperature"),
            humidity=data.get("humidity"),
            timestamp=int(time.time())
        )
        db.session.add(new_entry)
        db.session.commit()
        return jsonify({"message": "Veri kaydedildi"}), 200
    except Exception as e:
        return jsonify({"error": str(e)}), 400

@app.route("/get-data", methods=["GET"])
def get_latest_data():
    try:
        latest_entry = SensorData.query.order_by(SensorData.timestamp.desc()).first()
        if latest_entry:
            return jsonify({
                "id": latest_entry.id,
                "device_id": latest_entry.device_id,
                "temperature": latest_entry.temperature,
                "humidity": latest_entry.humidity,
                "timestamp": latest_entry.timestamp
            })
        else:
            return jsonify({"message": "Veri yok"}), 404
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
