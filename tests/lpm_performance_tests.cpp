#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <ctime>
#include <map>
#include "libpatricia/patricia.h"

int main() {
    patricia_tree_t *lookup_tree;
    lookup_tree = New_Patricia(32);
    make_and_lookup(lookup_tree, "46.36.216.0/21");
    make_and_lookup(lookup_tree, "159.253.16.0/21");
    make_and_lookup(lookup_tree, "5.45.112.0/21");
    make_and_lookup(lookup_tree, "5.45.120.0/21");
    make_and_lookup(lookup_tree, "5.101.112.0/21");
    make_and_lookup(lookup_tree, "5.101.120.0/21");
    make_and_lookup(lookup_tree, "185.4.72.0/22");
    make_and_lookup(lookup_tree, "181.114.240.0/20");
    make_and_lookup(lookup_tree, "193.42.142.0/24");

    prefix_t prefix_for_check_adreess;
    prefix_for_check_adreess.family = AF_INET;
    prefix_for_check_adreess.bitlen = 32;
    patricia_node_t* found_patrica_node = NULL;
    //prefix_for_check_adreess.add.sin.s_addr = 123123123;

    std::map <unsigned int, bool> lpm_cache;
    // Without cache: 16.7 million of operations 

    int i_iter = 10;
    // Million operations
    int j_iter = 1000000;

    printf("Preallocate table\n");
    // Iterate over all our IP addresses
    for (int j = 0; j < j_iter; j++) {
        for (int i = 0; i < i_iter; i++) {
            lpm_cache[i*j] = true;
        }
    }  

    printf("Start tests\n");
    timespec start_time;
    clock_gettime(CLOCK_REALTIME, &start_time);

    for (int j = 0; j < j_iter; j++) {
        for (int i = 0; i < i_iter; i++) {
            // Pseudo IP
            prefix_for_check_adreess.add.sin.s_addr = i*j;

            std::map <unsigned int, bool>::iterator itr = lpm_cache.find(i*j);

            if (itr !=  lpm_cache.end()) {
                // found it!
            } else {
                // cache miss
                // bool result = patricia_search_best(lookup_tree, &prefix_for_check_adreess) != NULL;
                //lpm_cache[i*j] = result;
                // not found!
            }
        }
    }

    timespec finish_time;
    clock_gettime(CLOCK_REALTIME, &finish_time);

    unsigned long used_seconds = finish_time.tv_sec - start_time.tv_sec;
    unsigned long total_ops = i_iter*j_iter;
    float megaops_per_second = (float)total_ops/(float)used_seconds/1000000;

    printf("Total time is %d seconds total ops: %d million of ops per second: %.1f\n", used_seconds, total_ops, megaops_per_second);

    Destroy_Patricia(lookup_tree,    (void_fn_t)0);
}

