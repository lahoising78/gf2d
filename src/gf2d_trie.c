#include "gf2d_trie.h"
#include "gfc_types.h"
#include "simple_logger.h"

void gf2d_trie_node_free(TrieNode *node);

int gf2d_trie_get_index(char c)
{
    return tolower(c) - 'a';
}

Trie gf2d_trie_new()
{
    Trie trie = {0};
    slog("create trie");
    trie.root = (TrieNode*)gfc_allocate_array(sizeof(TrieNode), 1);
    return trie;
}

uint8_t gf2d_trie_insert(Trie *trie, const char *key, void *val)
{
    TrieNode *cur = NULL;
    int i, c;
    size_t len;

    if(!trie || !key || !val) return 0;

    slog("insert %s in trie", key);

    cur = trie->root;
    len = strlen(key);

    for(i = 0; i < len; i++)
    {
        c = gf2d_trie_get_index(key[i]);
        if( !cur ) return 0;

        if( !cur->children[c] )
        {
            cur->children[c] = (TrieNode*)gfc_allocate_array(sizeof(TrieNode), 1);
            cur->children[c]->parent = cur;
        }

        cur = cur->children[c];
    }

    if( cur->leaf ) 
    {
        slog("Cannot insert in key %s. It is already in use", key);
        return 0;
    }

    cur->leaf = val;
    return 1;
}

void *gf2d_trie_remove(Trie *trie, const char *key)
{
    TrieNode *cur = NULL;
    int i, c;
    size_t len;
    
    if(!trie || !key) return 0;
    
    cur = trie->root;
    len = strlen(key);
    for(i = 0; i < len; i++)
    {
        c = gf2d_trie_get_index(key[i]);
        
        if(!cur->children[c])
            return NULL;
        
        cur = cur->children[c];
    }

    cur->parent->children[c] = NULL;
    return cur->leaf;
}

void gf2d_trie_free(Trie *trie)
{
    if(!trie) return;

    gf2d_trie_node_free(trie->root);
    if(trie->root) free( trie->root );
}

void gf2d_trie_node_free(TrieNode *node)
{
    int i;
    if(!node) return;

    slog("free trie node");

    for(i = 0; i < GF2D_LETTERS_IN_ALPHABET; i++)
    {
        if( !node->children[i] ) continue;
        gf2d_trie_node_free( node->children[i] );
        free( node->children[i] );
    }
}