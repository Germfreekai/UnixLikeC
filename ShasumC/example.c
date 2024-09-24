#include <stdio.h>
#include <openssl/evp.h>

#define BUFFER_SIZE 32768

void compute_sha256(FILE *file, unsigned char *hash) {
    EVP_MD_CTX *mdctx;
    unsigned char buffer[BUFFER_SIZE];
    size_t bytesRead;

    // Create and initialize the context
    if((mdctx = EVP_MD_CTX_new()) == NULL) {
        perror("EVP_MD_CTX_new");
        return;
    }

    // Initialize the digest operation
    if(1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL)) {
        perror("EVP_DigestInit_ex");
        EVP_MD_CTX_free(mdctx);
        return;
    }

    // Read the file and update the digest
    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file)) != 0) {
        if(1 != EVP_DigestUpdate(mdctx, buffer, bytesRead)) {
            perror("EVP_DigestUpdate");
            EVP_MD_CTX_free(mdctx);
            return;
        }
    }

    // Finalize the digest
    unsigned int hash_len;
    if(1 != EVP_DigestFinal_ex(mdctx, hash, &hash_len)) {
        perror("EVP_DigestFinal_ex");
    }

    // Clean up
    EVP_MD_CTX_free(mdctx);
}

void print_hash(unsigned char *hash) {
    for (int i = 0; i < EVP_MD_size(EVP_sha256()); i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    compute_sha256(file, hash);
    fclose(file);

    printf("SHA-256 hash of %s: ", argv[1]);
    print_hash(hash);

    return 0;
}
