#pragma once

#include <QByteArray>

class CryptoUtils
{
public:
    // Encrypts data using password-derived AES-256-CBC key
    static QByteArray encrypt(const QByteArray &data, const QByteArray &password);

    // Decrypts data using password-derived AES-256-CBC key
    static QByteArray decrypt(const QByteArray &encryptedData, const QByteArray &password);

private:
    static QByteArray deriveKey(const QByteArray &password, const QByteArray &salt, int keyLen = 32, int iterations = 100000);
};
