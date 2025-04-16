from sqlalchemy import create_engine, text

# Bağlantı bilgileri
server = "DESKTOP-ACMBM8M\\SQLEXPRESS"
database = "dinamik_kup"
username = "sa"
password = "Merkotech22."

# Bağlantı stringi
connection_string = (
    f"mssql+pyodbc://{username}:{password}@{server}/{database}"
    "?driver=ODBC+Driver+17+for+SQL+Server"
)

# Engine oluştur
engine = create_engine(connection_string)

print("🟡 Bağlantı deneniyor...")

try:
    with engine.connect() as conn:
        print("✅ SQL Server bağlantısı başarılı!")

        # Basit bir sorgu
        result = conn.execute(text("SELECT TOP 5 UserID, UserName FROM tblUser"))
        print("🟢 İlk 5 kullanıcı:")
        for row in result:
            print(f"ID: {row.UserID} | Kullanıcı Adı: {row.UserName}")

except Exception as e:
    print("❌ Hata oluştu:", e)
