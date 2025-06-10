#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "../ui/ui.h"

#define MAX_USER_COUNT 500
#define MAX_USERNAME_LENGTH 64
#define MAX_PASSWORD_LENGTH 129

#define BASE_IP "http://192.168.3.4:5000"

#define USER_API_URL BASE_IP "/users"
#define BANTLAR_API_URL BASE_IP "/get-bantlar"
#define OPERATION_API_BASE BASE_IP "/operations"
#define POST_DATA_API_URL BASE_IP "/post-data"
#define ORDER_API_URL BASE_IP "/orders"
#define TAG "HTTP"
#define SHA512_SALT "volkan"

typedef struct {
    int id;
    char username[MAX_USERNAME_LENGTH];
    char hashed_password[MAX_PASSWORD_LENGTH];  
    bool aktif;
} User;

extern User user_list[MAX_USER_COUNT];
extern int user_count;

void fetch_bantlar(void);
void fetch_user_list(void);
bool validate_user_credentials(const char *username, const char *password);
//void hash_password_sha512(const char *password, const char *salt, char *output_hex);
void hash_password_sha512_base64(const char *password, const char *salt, char *output_base64, size_t output_len);
void fetch_orders_for_bant(int bant_id);
void fetch_operations_by_order(int order_id);

// Yeni eklenen POST fonksiyonu
/**
 * @brief  Sayaç bilgilerini API'ye POST eder
 * @param  user_id       Kullanıcı ID'si
 * @param  machine_id    Makine ID'si (string)
 * @param  operation_id  Operasyon ID'si
 * @param  counter       Tamamlanan iş adedi
 */
void post_operation_data(int user_id, const char *machine_id, int operation_id, int counter);