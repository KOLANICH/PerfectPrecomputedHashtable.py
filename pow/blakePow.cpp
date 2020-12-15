#include <iostream>
//#include <unordered_set>
//#include <set>
#include <algorithm>
#include <array>
#include <thread>
#include <tuple>
#include <vector>

#include <signal.h>
#include <stdlib.h>

#include "./blake2s.cpp"

#include "./strings.cpp"

constexpr auto countOfStrings = end(strings) - begin(strings);

#include "./reducers.cpp"


using HashT = uint32_t;
using NonceT = uint32_t;
using ReducedHashT = uint8_t;
using ReducerIndexT = uint8_t;

struct LoopIterResult {
	NonceT nonce = 0xFFFFFFFF;
	HashT span = 0xFFFFFFFF;
	ReducedHashT reducedSpan = 0xFF;
	ReducerIndexT reducer = 0xFF;
};

LoopIterResult tryNonce(NonceT nonce) {
	blake2s_state protoState = {{0}};
	blake2s_init_key(&protoState, sizeof(HashT), &nonce, sizeof(nonce));

	//std::cout  << "Nonce: " << std::hex << nonce << std::endl;
	LoopIterResult res {.nonce = nonce};

	std::array<uint32_t, countOfStrings> hashes;
	/*
	std::vector<uint32_t> hashes;
	hashes.reserve(countOfStrings);
	hashes.resize(countOfStrings);*/

	uint8_t i = 0;
	for(auto &s: strings) {
		HashT hash;
		blake2s_state iterState = protoState;
		blake2s_update(&iterState, (const uint8_t *) s.data(), s.size());
		blake2s_final(&iterState, &hash, sizeof(hash));
		hashes[i] = hash;
		++i;
	}
	std::sort(begin(hashes), end(hashes));
	uint32_t prev = 0;
	bool incomplete = false;
	for(auto e: hashes) {
		//std::cout << e << " " << prev << std::endl;
		if(e == prev) {
			incomplete = true;
			break;
		}
		prev = e;
	}
	if(!incomplete) {
		res.span = hashes[hashes.size() - 1] - hashes[0];

		std::array<uint8_t, countOfStrings> reducedHashes;
		/*std::vector<uint8_t> reducedHashes;
		reducedHashes.reserve(countOfStrings);
		reducedHashes.resize(countOfStrings);*/

		auto j = 0;

		for(auto r: reductors) {
			std::transform(begin(hashes), end(hashes), begin(reducedHashes), r);
			std::sort(begin(reducedHashes), end(reducedHashes));

			bool partialIncomplete = false;

			uint8_t prev = 0;
			for(auto e: reducedHashes) {
				//std::cout << (uint16_t) e << " " << (uint16_t) prev << std::endl;
				if(e == prev) {
					partialIncomplete = true;
					break;
				}
				prev = e;
			}
			if(!partialIncomplete) {
				uint8_t reducedSpan = reducedHashes[reducedHashes.size() - 1] - reducedHashes[0];
				//std::cout << std::dec << "reducer " << j << " complete! Reduced span: " << (uint16_t) reducedSpan << " " << (uint16_t) reducedHashes[0] << " " << (uint16_t) reducedHashes[reducedHashes.size() - 1] << std::endl;
				if(res.reducedSpan > reducedSpan) {
					res.reducedSpan = reducedSpan;
					res.reducer = j;
				}
			}
			++j;
		}
		/*if(res.reducedSpan != 0xFF){
			std::cout << std::dec << res.nonce << " " << std::hex << res.span << " min reducer " << (uint16_t) res.reducer << " reduced_span: " << (uint16_t) res.reducedSpan << std::endl;
		}*/
	} else {
		//std::cout << std::dec << res.nonce << " incomplete" << std::endl;
	}
	return res;
}

struct Space {
	NonceT start, stop;
};

using ResultsT = std::vector<LoopIterResult>;

LoopIterResult nonceSearchLoop(Space s) {
	LoopIterResult res;

	for(NonceT nonce = s.start; nonce < s.stop; ++nonce) {
		auto iterRes = tryNonce(nonce);
		if(iterRes.reducedSpan < res.reducedSpan || (iterRes.reducedSpan == res.reducedSpan && iterRes.span < res.span)) {
			res = iterRes;
		}
	}
	return res;
}

void singleSearchingThreadFunction(uint8_t id, ResultsT *resultsPtr, Space s) {
	(*resultsPtr)[id] = nonceSearchLoop(s);
}


extern "C" void ctrlCHandler(int sig){
	std::cout << "Ctrl+C fired" << std::endl;
	exit(-1);
}

int main() {
	uint16_t procsCount = std::thread::hardware_concurrency();
	uint16_t lastProc = procsCount - 1;

	std::cout << "Started" << std::endl;
	std::cout << "Count: " << countOfStrings << std::endl;
	std::cout << "Concurrency: " << procsCount << std::endl;

	signal(SIGINT, ctrlCHandler);

	Space s {
		.start = 0x90000000,
		.stop  = 0x90000100
	};


	NonceT spaceSize = s.stop - s.start;
	size_t shareSize = spaceSize / procsCount;

	LoopIterResult res;
	if(procsCount > 1) {
		std::vector<std::thread> threadList;
		threadList.reserve(procsCount);
		std::vector<LoopIterResult> results;
		results.reserve(procsCount);
		results.resize(procsCount);

		for(uint8_t i = 0; i < lastProc; ++i) {
			NonceT start = s.start + shareSize * i;
			Space curSpace = {
				.start = start,
				.stop = static_cast<NonceT>(start + shareSize)};
			threadList.emplace_back(std::thread(singleSearchingThreadFunction, i, &results, curSpace));
		}
		auto lastStart = s.start + shareSize * lastProc;
		threadList.emplace_back(std::thread(singleSearchingThreadFunction, lastProc, &results, Space {.start = static_cast<NonceT>(lastStart), .stop = spaceSize}));
		std::for_each(threadList.begin(), threadList.end(), std::mem_fn(&std::thread::join));

		for(auto iterRes: results) {
			if(iterRes.reducedSpan < res.reducedSpan) {
				res = iterRes;
			}
		}
	} else {
		res = nonceSearchLoop(s);
	}

	if(res.reducedSpan != 0xFF) {
		std::cout << std::hex << "{\"nonce\": 0x" << res.nonce << ", \"span\": 0x" << res.span << std::dec << ", \"reducer\": " << (uint16_t) res.reducer << ", \"reduced_span\": " << (uint16_t) res.reducedSpan << "}" << std::endl;
	}

	/*{
		uint32_t nonce = 0xf700fdc4;
		blake2s_state protoState = {{0}};
		blake2s_init_key(&protoState, sizeof(HashT), &nonce, sizeof(nonce));

		//std::cout  << "Nonce: " << std::hex << nonce << std::endl;

		uint8_t i = 0;
		for(auto &s: strings) {
			HashT hash;
			blake2s_state iterState = protoState;
			blake2s_update(&iterState, (const uint8_t *) s.data(), s.size());
			blake2s_final(&iterState, &hash, sizeof(hash));
			std::cout << s << " " << hash << std::endl;
		}
	}*/

	return 0;
}
