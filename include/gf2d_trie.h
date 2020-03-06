#ifndef _GF2D_TRIE_H_
#define _GF2D_TRIE_H_

#include <stdlib.h>
#include <stdint.h>

#define GF2D_LETTERS_IN_ALPHABET 128-32

typedef struct trie_node_s
{
    struct trie_node_s *parent;
    struct trie_node_s *children[GF2D_LETTERS_IN_ALPHABET];
    void *leaf;
} TrieNode;

typedef struct
{
    TrieNode *root;
} Trie;

Trie gf2d_trie_new();
uint8_t gf2d_trie_insert(Trie *trie, const char *key, void *val);
void *gf2d_trie_remove(Trie *trie, const char *key);
void gf2d_trie_free(Trie *trie);
void *gf2d_trie_get(Trie *trie, const char *key);

#endif