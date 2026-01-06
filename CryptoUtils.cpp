#include "CryptoUtils.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdexcept>

QByteArray CryptoUtils::deriveKey(const QByteArray &password, const QByteArray &salt, int keyLen, int iterations)
{
    QByteArray key(keyLen, 0);
    if (!PKCS5_PBKDF2_HMAC(password.constData(), password.size(),
                           reinterpret_cast<const unsigned char*>(salt.constData()), salt.size(),
                           iterations, EVP_sha256(),
                           keyLen, reinterpret_cast<unsigned char*>(key.data())))
    {
        throw std::runtime_error("Key derivation failed");
    }
    return key;
}

QByteArray CryptoUtils::encrypt(const QByteArray &data, const QByteArray &password)
{
    // Generate random salt and IV
    QByteArray salt(16, 0);
    QByteArray iv(16, 0);
    if (!RAND_bytes(reinterpret_cast<unsigned char*>(salt.data()), salt.size()) ||
        !RAND_bytes(reinterpret_cast<unsigned char*>(iv.data()), iv.size()))
    {
        throw std::runtime_error("Random generation failed");
    }

    QByteArray key = deriveKey(password, salt, 32);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("Failed to create cipher context");

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                                reinterpret_cast<const unsigned char*>(key.constData()),
                                reinterpret_cast<const unsigned char*>(iv.constData())))
    {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("EncryptInit failed");
    }

    QByteArray cipherData;
    cipherData.resize(data.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));
    int outLen1 = 0;
    if (1 != EVP_EncryptUpdate(ctx,
                               reinterpret_cast<unsigned char*>(cipherData.data()), &outLen1,
                               reinterpret_cast<const unsigned char*>(data.constData()), data.size()))
    {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("EncryptUpdate failed");
    }

    int outLen2 = 0;
    if (1 != EVP_EncryptFinal_ex(ctx,
                                 reinterpret_cast<unsigned char*>(cipherData.data()) + outLen1, &outLen2))
    {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("EncryptFinal failed");
    }

    cipherData.resize(outLen1 + outLen2);
    EVP_CIPHER_CTX_free(ctx);

    // Prepend salt + IV to ciphertext
    QByteArray finalData;
    finalData.reserve(salt.size() + iv.size() + cipherData.size());
    finalData.append(salt);
    finalData.append(iv);
    finalData.append(cipherData);

    return finalData;
}

QByteArray CryptoUtils::decrypt(const QByteArray &encryptedData, const QByteArray &password)
{
    if (encryptedData.size() < 32)
        throw std::runtime_error("Encrypted data too short");

    QByteArray salt = encryptedData.left(16);
    QByteArray iv = encryptedData.mid(16, 16);
    QByteArray cipherData = encryptedData.mid(32);

    QByteArray key = deriveKey(password, salt, 32);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("Failed to create cipher context");

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                                reinterpret_cast<const unsigned char*>(key.constData()),
                                reinterpret_cast<const unsigned char*>(iv.constData())))
    {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("DecryptInit failed");
    }

    QByteArray plainData;
    plainData.resize(cipherData.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));
    int outLen1 = 0;
    if (1 != EVP_DecryptUpdate(ctx,
                               reinterpret_cast<unsigned char*>(plainData.data()), &outLen1,
                               reinterpret_cast<const unsigned char*>(cipherData.constData()), cipherData.size()))
    {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("DecryptUpdate failed");
    }

    int outLen2 = 0;
    if (1 != EVP_DecryptFinal_ex(ctx,
                                 reinterpret_cast<unsigned char*>(plainData.data()) + outLen1, &outLen2))
    {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("DecryptFinal failed: wrong password or corrupted file");
    }

    plainData.resize(outLen1 + outLen2);
    EVP_CIPHER_CTX_free(ctx);

    return plainData;
}
