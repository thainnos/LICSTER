import sqlite3
import os

path = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))
db_path = os.path.join(path, "software/instance/app.sqlite")
conn = None


def get_db():
    conn = sqlite3.connect(db_path)
    return conn


def close_db():
    conn.close()


def reset_db():
    db = get_db()
    db.execute('DROP TABLE IF EXISTS snort')
    db.commit()

    db.execute("CREATE TABLE IF NOT EXISTS snort(id INTEGER PRIMARY"
               " KEY AUTOINCREMENT, snort_type TEXT NOT NULL, "
               "snort_classification TEXT NOT NULL, snort_priority"
               " INTEGER NOT NULL, snort_datetime TEXT NOT NULL)")
    db.commit()
# reset wird dann als eigener Knopf in die webapp implementiert
# doku mit einführen receiver kann jeder sein
# # -> damit nicht es nicht im spam ordner landet
# muss es jedoch ein kontankt sein
# es wird noch ein db table mit den receiver emails eingeführt
# dieser table ist dann auch im admin dashboard bearbeitbar
##
