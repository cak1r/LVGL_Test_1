#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "../ui/ui.h"

#define MAX_USER_COUNT 500
#define MAX_USERNAME_LENGTH 64
#define MAX_PASSWORD_LENGTH 129

#define USER_API_URL "http://192.168.1.35:5000/users"
#define BANTLAR_API_URL "http://192.168.1.35:5000/get-bantlar"

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