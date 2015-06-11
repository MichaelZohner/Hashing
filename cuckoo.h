/*
 * cuckoo.h
 *
 *  Created on: Oct 7, 2014
 *      Author: mzohner
 */

#ifndef CUCKOO_H_
#define CUCKOO_H_

#include "hashing_util.h"


//TEST CHANGE
//TEST CHANGE PSI->Hashing
//This is a test
#define MAX_ITERATIONS 1024
//#define DEBUG_CUCKOO
#ifdef TEST_UTILIZATION
#define COUNT_FAILS
#endif
//#define DOUBLE_TABLE
//#define TEST_CHAINLEN

#ifdef TEST_CHAINLEN
static uint64_t chain_cnt[MAX_ITERATIONS];
void print_chain_cnt();
#endif

struct cuckoo_entry_ctx {
	//id of the element in the source set
	uint32_t eleid;
	//addresses the bin of the cuckoo entry in the cuckoo table, will only work for up to 2^{32} bins
	uint32_t address[NUM_HASH_FUNCTIONS];
	//the value of the entry
	uint8_t* val;
	//which position is the entry currently mapped to
	uint32_t pos;
#ifdef DEBUG_CUCKOO
	uint8_t* element;
#endif
};



struct cuckoo_entry_gen_ctx {
	//starting position in the generation process
	uint32_t startpos;
	//end position of entries that are generated by this thread
	uint32_t endpos;
	//input elements
	uint8_t* elements;
	//pointer to the cuckoo entries
	cuckoo_entry_ctx* cuckoo_entries;
	hs_t* hs;
};


//returns a cuckoo hash table with the first dimension being the bins and the second dimension being the pointer to the elements
#ifndef TEST_UTILIZATION
uint8_t*
#else
uint32_t
#endif
cuckoo_hashing(uint8_t* elements, uint32_t neles, uint32_t nbins, uint32_t bitlen, uint32_t* outbitlen, uint32_t* nelesinbin,
		uint32_t* perm,	uint32_t ntasks, prf_state_ctx* prf_state);
//routine for generating the entries, is invoked by the threads
void *gen_cuckoo_entries(void *ctx);
inline void gen_cuckoo_entry(uint8_t* in, cuckoo_entry_ctx* out, hs_t* hs, uint32_t ele_id);
#ifdef DOUBLE_TABLE
inline bool insert_element(cuckoo_entry_ctx*** ctable, cuckoo_entry_ctx* element, uint32_t max_iterations);
#else
inline bool insert_element(cuckoo_entry_ctx** ctable, cuckoo_entry_ctx* element, uint32_t max_iterations);
#endif



#endif /* CUCKOO_H_ */
