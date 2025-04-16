import pyodbc

print("starting")
try:
    conn = pyodbc.connect(
        'DRIVER={ODBC Driver 17 for SQL Server};'
        'SERVER=DESKTOP-ACMBM8M\\SQLEXPRESS;'
        'DATABASE=dinamik_kup;'
        'UID=sa;'
        'PWD=Merkotech22.'
    )
    print("✅ Bağlantı başarılı!")

    cursor = conn.cursor()
    cursor.execute("SELECT TOP 5 UserID, UserName FROM tblUser")  # Örnek sorgu
    rows = cursor.fetchall()

    print(f" Toplam kayıt: {len(rows)}")
    print(" İlk 5 kullanıcı:")
    for row in rows[:5]:
        print(f"ID: {row[0]} | Kullanıcı Adı: {row[1]}")


    conn.close()

except Exception as e:
    print("❌ Bağlantı hatası:", e)
