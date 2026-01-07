# 📔 Matt's Journal & Log Book

Hi all! This is a personal project I built to help track thoughts and organize life events. Unlike a standard linear diary, this app allows you to categorize entries into specific **Journals** (e.g., "First Year of College," "Vegas Trip," or "Miscellaneous").

I am always trying to improve on my skills as a programmer, so I welcome any and all feedback!

---
## 📸 Preview
This is one example of its use(from me) to keep track of gym progress.
Note: UI is tron inspired
<img width="1440" alt="App Screenshot" src="https://github.com/user-attachments/assets/f96f937b-ca4e-4da6-8ce8-3ddec1d13d8f" />
<img width="1440" height="900" alt="Screenshot 2026-01-07 at 12 09 42 PM" src="https://github.com/user-attachments/assets/003b4e97-fcac-4049-838a-a60cbbd826fe" />

---

## ✨ Features
* **Hierarchical Organization:** Create multiple journals to separate different parts of your life.
* **Secure Entries:** Password protection to keep your thoughts private.

## 🛠️ Tech Stack
* **Language:** C++
* **Framework:** Qt
* **Security:** OpenSSL
  * Encryption: AES-256-CBC
  * Key Derivation: PBKDF2 with HMAC-SHA256 (for secure password handling)
  * Entropy: Cryptographically secure random salt and IV generation
* **Data Persistence:** Encrypted JSON stored locally.

## 📥 Download & Installation
1.  Navigate to the **[Releases](https://github.com/Khraen/Matt-s-Journal-Log-book/releases/tag/v.0.1.0)** section of this repository.
2.  Download the latest version for macOS.

## ⚠️ Important Notes
* **Platform Support:** This app was developed on macOS and is currently the **only** supported platform.
* **Password Security:** There is **no password recovery option**. Please choose a password you will remember, as data cannot be recovered if the password is lost.

---

## 👨‍💻 Learning Goals
This project was a way for me to practice:
* Managing file systems and data storage
* Designing a user interface to go with my program.
* Handling user authentication and security.

---
*Created by Matthew Holgin — Hope you enjoy!*
