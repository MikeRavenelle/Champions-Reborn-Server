### Champions Reborn Emulated Server
A replacement server aimed at restoring **online gameplay** for **Champions of Norrath** and **Champions: Return To Arms** for the **PlayStation 2**.

The server is currently a work in progress.

## Support:
| Game                          | Status             |
|-------------------------------|--------------------|
| Champions of Norrath          | ✅ Supported       |
| Champions: Return To Arms     | ✅ Supported       |

---

## 🔧 Dependencies

- [SQLite3](https://www.sqlite.org/index.html) — Used for session and user data storage

## Linux / Docker

```
PUBLIC_IP=<address clients connect to> docker compose up -d --build
```

Data (SQLite DB, `config.ini`, logs) lives in the `champions-data` volume. Ports: TCP 40810-40830 (gateways), 40900, 40910; UDP 10101.
