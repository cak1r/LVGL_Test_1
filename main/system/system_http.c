#include "system_http.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "cJSON.h"
#include "mbedtls/sha512.h"
#include "mbedtls/base64.h"
#include <string.h>
#include <stdlib.h>



User user_list[MAX_USER_COUNT];
int user_count = 0;
char current_logged_user[MAX_USERNAME_LENGTH] = "";


Operation operation_list[MAX_OPERATION_COUNT];
int operation_count = 0;

void post_operation_data(int user_id, const char *machine_id, int operation_id, int counter) {
    uint32_t start_time = esp_log_timestamp();  // İşlem başlangıcını al

    // 1) Build JSON payload
    time_t now = time(NULL);                  // NTP‐synced timestamp
    long ts = (long)now;

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        ESP_LOGE(TAG, "Failed to allocate JSON object");
        return;
    }
    
    // JSON verisini oluştur
    ESP_LOGI(TAG, "Preparing JSON payload:");
    ESP_LOGI(TAG, "user_id: %d", user_id);
    ESP_LOGI(TAG, "machine_id: %s", machine_id);
    ESP_LOGI(TAG, "operation_id: %d", operation_id);
    ESP_LOGI(TAG, "timestamp: %ld", ts);
    ESP_LOGI(TAG, "counter: %d", counter);

    cJSON_AddNumberToObject(root, "user_id", user_id);
    cJSON_AddStringToObject(root, "machine_id", machine_id);
    cJSON_AddNumberToObject(root, "operation_id", operation_id);
    cJSON_AddNumberToObject(root, "timestamp", ts);
    cJSON_AddNumberToObject(root, "counter", counter);

    char *body = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    if (!body) {
        ESP_LOGE(TAG, "Failed to print JSON");
        return;
    }

    // Debug: print full JSON payload
    ESP_LOGI(TAG, "Full JSON payload: %s", body);

    // 2) Configure and send HTTP POST
    esp_http_client_config_t config = {
        .url         = POST_DATA_API_URL,
        .method      = HTTP_METHOD_POST,
        .timeout_ms  = 5000,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client) {
        ESP_LOGE(TAG, "esp_http_client_init failed");
        free(body);
        return;
    }

    // Set header and send HTTP POST
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, body, strlen(body));

    esp_err_t err = esp_http_client_open(client, strlen(body));
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "HTTP open failed: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        free(body);
        return;
    }

    // Send the request
    int write_len = esp_http_client_write(client, body, strlen(body));
    ESP_LOGI(TAG, "Bytes written: %d", write_len);

    // Get response
    int status = esp_http_client_get_status_code(client);
    ESP_LOGI(TAG, "POST %s → %d", POST_DATA_API_URL, status);

    // Optional: read response body for more details
    char response_buffer[512];
    int read_len = esp_http_client_read(client, response_buffer, sizeof(response_buffer) - 1);
    if (read_len > 0) {
        response_buffer[read_len] = '\0';
        ESP_LOGI(TAG, "Response body: %s", response_buffer);
    }

    // 3) Cleanup
    esp_http_client_cleanup(client);
    free(body);

    // İşlem süresini hesapla
    uint32_t end_time = esp_log_timestamp();  // İşlem tamamlandıktan sonra zaman al
    ESP_LOGI(TAG, "POST işlemi tamamlandı, Süre: %ld ms", end_time - start_time);
}


void fetch_operations_by_order(int order_id) {
    operation_count = 0;
    char url[256];
    snprintf(url, sizeof(url), "%s?order_id=%d", OPERATION_API_BASE, order_id);

    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_GET,
        .timeout_ms = 5000,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (esp_http_client_open(client, 0) != ESP_OK) {
        ESP_LOGE(TAG, "❌ HTTP bağlantısı kurulamadı.");
        esp_http_client_cleanup(client);
        return;
    }

    int content_length = esp_http_client_fetch_headers(client);
    if (content_length <= 0) content_length = 4096;

    char *buffer = malloc(content_length + 1);
    if (!buffer) {
        ESP_LOGE(TAG, "❌ Bellek yetersiz!");
        esp_http_client_cleanup(client);
        return;
    }

    int read_len = esp_http_client_read_response(client, buffer, content_length);
    if (read_len <= 0) {
        ESP_LOGE(TAG, "❌ HTTP yanıtı okunamadı.");
        free(buffer);
        esp_http_client_cleanup(client);
        return;
    }
    buffer[read_len] = '\0';

    ESP_LOGD(TAG, "📦 HTTP yanıtı: %s", buffer);

    cJSON *root = cJSON_Parse(buffer);
    if (!root) {
        ESP_LOGE(TAG, "❌ JSON ayrıştırma hatası");
        free(buffer);
        esp_http_client_cleanup(client);
        return;
    }

    cJSON *arr = cJSON_IsArray(root) ? root : cJSON_GetObjectItem(root, "operations");
    if (!cJSON_IsArray(arr)) {
        ESP_LOGW(TAG, "❌ JSON beklenen dizi formatında değil");
        cJSON_Delete(root);
        free(buffer);
        esp_http_client_cleanup(client);
        return;
    }

    int index = 0;
    cJSON *item;
    cJSON_ArrayForEach(item, arr) {
        if (index >= MAX_OPERATION_COUNT) break;

        cJSON *id        = cJSON_GetObjectItem(item, "id");
        cJSON *name      = cJSON_GetObjectItem(item, "name");
        cJSON *unit_time = cJSON_GetObjectItem(item, "unit_time");
        cJSON *cut_count = cJSON_GetObjectItem(item, "cut_count");
        cJSON *unit_time_tol = cJSON_GetObjectItem(item, "unit_time_tol"); // Fetch the new field

        // Zorunlu alan kontrolü: name, unit_time, cut_count
        if (!name || !cJSON_IsString(name) ||
            !unit_time || !cJSON_IsNumber(unit_time) ||
            !cut_count || !cJSON_IsNumber(cut_count)) {
            ESP_LOGW(TAG, "⏭️ Geçersiz JSON öğesi, atlanıyor");
            continue;
        }

        // id opsiyonel: varsa ata, yoksa index veya varsayılan değer
        if (id && cJSON_IsNumber(id)) {
            operation_list[index].id = id->valueint;
        } else {
            operation_list[index].id = index;
        }
        operation_list[index].order_id = order_id;

        // Bellek ayırma ve kopyalama
        operation_list[index].name = malloc(strlen(name->valuestring) + 1);
        if (operation_list[index].name) {
            strcpy(operation_list[index].name, name->valuestring);
        } else {
            ESP_LOGW(TAG, "❌ Ad için bellek ayırma başarısız");
        }

        operation_list[index].unit_time = unit_time->valueint;
        operation_list[index].cut_count = cut_count->valueint;

        // Yeni eklenen unit_time_tol alanını kontrol et ve ata
        if (unit_time_tol && cJSON_IsNumber(unit_time_tol)) {
            operation_list[index].unit_time_tol = unit_time_tol->valueint;
        } else {
            operation_list[index].unit_time_tol = 0; // Varsayılan değer
        }

        index++;
    }
    operation_count = index;

    cJSON_Delete(root);
    free(buffer);
    esp_http_client_cleanup(client);
}



void fetch_orders_for_bant(int bant_id) {
    order_count = 0;
    char url[128];
   
    snprintf(url, sizeof(url), "%s?bant_id=%d", ORDER_API_URL, bant_id);

    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_GET,
        .timeout_ms = 5000
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (esp_http_client_open(client, 0) != ESP_OK) {
        ESP_LOGE("HTTP", "❌ Order API bağlanamadı");
        esp_http_client_cleanup(client);
        return;
    }

    int content_length = esp_http_client_fetch_headers(client);
    if (content_length <= 0) content_length = 4096;

    char *buffer = malloc(content_length + 1);
    int read_len = esp_http_client_read_response(client, buffer, content_length);
    buffer[read_len] = '\0';

    cJSON *root = cJSON_Parse(buffer);
    if (cJSON_IsArray(root)) {
        int index = 0;
        cJSON *item;
        cJSON_ArrayForEach(item, root) {
            if (index >= MAX_ORDER_COUNT) break;

            cJSON *id = cJSON_GetObjectItem(item, "id");
            cJSON *code = cJSON_GetObjectItem(item, "code");
            cJSON *bant = cJSON_GetObjectItem(item, "bant_id");

            if (cJSON_IsString(code)) {
                order_list[index].id = id->valueint;
                order_list[index].bant_id = bant->valueint;
                strncpy(order_list[index].code, code->valuestring, MAX_ORDER_CODE_LENGTH - 1);
                order_list[index].code[MAX_ORDER_CODE_LENGTH - 1] = '\0';
                index++;
            }
        }
        order_count = index;
    }

    cJSON_Delete(root);
    free(buffer);
    esp_http_client_cleanup(client);
}


void fetch_bantlar(void) {
    ESP_LOGI("BANT", "🔄 Bant verisi çekiliyor...");
    bant_count = 0;

    esp_http_client_config_t config = {
        .url = BANTLAR_API_URL,
        .method = HTTP_METHOD_GET,
        .timeout_ms = 5000,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    if (esp_http_client_open(client, 0) != ESP_OK) {
        ESP_LOGE("BANT", "❌ API bağlantısı kurulamadı.");
        esp_http_client_cleanup(client);
        return;
    }

    int content_length = esp_http_client_fetch_headers(client);
    if (content_length <= 0) content_length = 4096;

    char *buffer = malloc(content_length + 1);
    int read_len = esp_http_client_read_response(client, buffer, content_length);
    buffer[read_len] = '\0';

    cJSON *root = cJSON_Parse(buffer);
    if (cJSON_IsArray(root)) {
        cJSON *item = NULL;
        int index = 0;
        cJSON_ArrayForEach(item, root) {
            if (index >= MAX_BANT_COUNT) break;

            cJSON *id = cJSON_GetObjectItem(item, "id");
            cJSON *tanım = cJSON_GetObjectItem(item, "tanım");

            if (cJSON_IsNumber(id) && cJSON_IsString(tanım)) {
                bant_list[index].bant_id = id->valueint;
                strncpy(bant_list[index].bant_tanim, tanım->valuestring, sizeof(bant_list[index].bant_tanim) - 1);
                index++;
            }
        }
        bant_count = index;
    }

    cJSON_Delete(root);
    free(buffer);
    esp_http_client_cleanup(client);

    ESP_LOGI("BANT", "✅ Toplam bant: %d", bant_count);
}

void hash_password_sha512_base64(const char *password, const char *salt, char *output_base64, size_t output_len) {
    unsigned char hash[64];  // SHA512 = 64 byte
    mbedtls_sha512_context ctx;

    // Şifre + salt birleşimi
    size_t pass_len = strlen(password);
    size_t salt_len = strlen(salt);
    size_t combined_len = pass_len + salt_len;

    unsigned char *combined = malloc(combined_len);
    if (!combined) {
        ESP_LOGE(TAG, "❌ Bellek ayrilamadi");
        return;
    }

    memcpy(combined, password, pass_len);
    memcpy(combined + pass_len, salt, salt_len);

    // SHA512 hashle
    mbedtls_sha512_init(&ctx);
    mbedtls_sha512_starts(&ctx, 0); // SHA512
    mbedtls_sha512_update(&ctx, combined, combined_len);
    mbedtls_sha512_finish(&ctx, hash);
    mbedtls_sha512_free(&ctx);
    free(combined);

    // Hash + salt birleşimi
    unsigned char hash_plus_salt[64 + 32]; // 64 hash + max 32 salt
    memcpy(hash_plus_salt, hash, 64);
    memcpy(hash_plus_salt + 64, salt, salt_len);

    size_t final_len = 64 + salt_len;

    // Base64 encode
    size_t encoded_len = 0;
    mbedtls_base64_encode((unsigned char *)output_base64, output_len, &encoded_len, hash_plus_salt, final_len);
    output_base64[encoded_len] = '\0';
}

void fetch_user_list(void) {
    user_count = 0;
    ESP_LOGI(TAG, "📡 Kullanıcı verisi çekiliyor...");

    esp_http_client_config_t config = {
        .url = USER_API_URL,
        .method = HTTP_METHOD_GET,
        .timeout_ms = 5000,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    if (esp_http_client_open(client, 0) != ESP_OK) {
        ESP_LOGE(TAG, "❌ HTTP bağlantısı kurulamadı.");
        esp_http_client_cleanup(client);
        return;
    }

    int content_length = esp_http_client_fetch_headers(client);
    if (content_length <= 0) content_length = 4096;

    char *buffer = malloc(content_length + 1);
    if (!buffer) {
        ESP_LOGE(TAG, "❌ Bellek yetersiz!");
        esp_http_client_cleanup(client);
        return;
    }

    int read_len = esp_http_client_read_response(client, buffer, content_length);
    buffer[read_len] = '\0';

    ESP_LOGI(TAG, "📏 Gelen veri uzunluğu: %d", read_len);
    //ESP_LOGI(TAG, "🧾 JSON:\n%s", buffer);

    // JSON parse
    cJSON *root = cJSON_Parse(buffer);
    if (!cJSON_IsArray(root)) {
        ESP_LOGE(TAG, "⚠️ JSON dizi değil");
        free(buffer);
        cJSON_Delete(root);
        esp_http_client_cleanup(client);
        return;
    }

    cJSON *item = NULL;
    int index = 0;
    cJSON_ArrayForEach(item, root) {
        if (index >= MAX_USER_COUNT) break;

        cJSON *id = cJSON_GetObjectItem(item, "id");
        cJSON *username = cJSON_GetObjectItem(item, "username");
        cJSON *password = cJSON_GetObjectItem(item, "hashed_password");
        cJSON *aktif = cJSON_GetObjectItem(item, "aktif");

        if (cJSON_IsString(username) && cJSON_IsString(password)) {
            user_list[index].id = cJSON_IsNumber(id) ? id->valueint : -1;
            strncpy(user_list[index].username, username->valuestring, MAX_USERNAME_LENGTH - 1);
            user_list[index].username[MAX_USERNAME_LENGTH - 1] = '\0';
            strncpy(user_list[index].hashed_password, password->valuestring, MAX_PASSWORD_LENGTH - 1);
            user_list[index].hashed_password[MAX_PASSWORD_LENGTH - 1] = '\0';
            user_list[index].aktif = cJSON_IsBool(aktif) ? cJSON_IsTrue(aktif) : true;

            //ESP_LOGI(TAG, "✅ Kullanıcı [%d]: %s", index + 1, user_list[index].username);
            index++;
        }
    }

    user_count = index;
    ESP_LOGI(TAG, "🎯 Toplam kullanıcı: %d", user_count);
        
    cJSON_Delete(root);
    free(buffer);
    esp_http_client_cleanup(client);
}

bool validate_user_credentials(const char *username, const char *password) {
    char hashed_input[129];
    hash_password_sha512_base64(password, SHA512_SALT, hashed_input, sizeof(hashed_input));

    for (int i = 0; i < user_count; i++) {
        if (user_list[i].aktif &&
            strcmp(user_list[i].username, username) == 0 &&
            strcmp(user_list[i].hashed_password, hashed_input) == 0) {
            ESP_LOGI("LOGIN","pass: %s",hashed_input);
            // store both the username *and* the id
            current_logged_user_id = user_list[i].id;
            strncpy(current_logged_user, username, MAX_USERNAME_LENGTH - 1);
            current_logged_user[MAX_USERNAME_LENGTH - 1] = '\0';
            return true;
        }
    }
    return false;
}
