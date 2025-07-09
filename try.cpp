#include <helib/helib.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include <random>

using namespace std;

int main(int argc, char *argv[]) {
    // Parameters
    size_t nslots = 64; // default
    long p = 4999; // plaintext prime
    long r = 1;     // lifting
    long bits = 300;
    long c = 2;
    long query_val = 42;

    if (argc > 1) nslots = stoi(argv[1]);
    if (argc > 2) query_val = stol(argv[2]);

    cout << "Parameters: slots=" << nslots << ", query=" << query_val << endl;

    // Initialize context
    helib::Context context = helib::ContextBuilder<helib::BGV>()
        .m(8192)
        .p(p)
        .r(r)
        .bits(bits)
        .c(c)
        .build();

    // Key Generation
    helib::SecKey secret_key(context);
    secret_key.GenSecKey();
    const helib::PubKey& public_key = secret_key;
    const helib::EncryptedArray& ea = context.getEA();

    // Prepare database (encrypted)
    size_t actual_slots = ea.size();
    if (nslots > actual_slots) {
        cerr << "Warning: Reducing nslots to fit context (" << actual_slots << " slots available)." << endl;
        nslots = actual_slots;
    }
    vector<long> data(nslots);

    iota(data.begin(), data.end(), 0);
    helib::Ptxt<helib::BGV> pt_data(context, data);
    helib::Ctxt ctxt_data(public_key);
    public_key.Encrypt(ctxt_data, pt_data);

    // Client prepares query
    auto t0 = chrono::high_resolution_clock::now();
    vector<long> query_vec(nslots, query_val);
    helib::Ptxt<helib::BGV> pt_query(context, query_vec);
    helib::Ctxt ctxt_query(public_key);
    public_key.Encrypt(ctxt_query, pt_query);
    auto t1 = chrono::high_resolution_clock::now();

    // Server evaluates: compute difference
    auto t2 = chrono::high_resolution_clock::now();
    helib::Ctxt ctxt_diff = ctxt_data;
    ctxt_diff -= ctxt_query;

    // Apply random mask
    vector<long> mask(nslots);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<long> dist(1, p - 1);
    for (auto &x : mask) x = dist(gen);
    helib::Ptxt<helib::BGV> pt_mask(context, mask);
    ctxt_diff.multByConstant(pt_mask);
    auto t3 = chrono::high_resolution_clock::now();

    // Client decrypts
    auto t4 = chrono::high_resolution_clock::now();
    helib::Ptxt<helib::BGV> pt_result(context);
    secret_key.Decrypt(pt_result, ctxt_diff);
    bool found = false;
    for (long i = 0; i < pt_result.size(); ++i) {
        if (pt_result[i] == 0) {
            found = true;
            break;
        }
    }
    auto t5 = chrono::high_resolution_clock::now();

    // Timing
    auto prep_ms = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
    auto eval_ms = chrono::duration_cast<chrono::milliseconds>(t3 - t2).count();
    auto verif_ms = chrono::duration_cast<chrono::milliseconds>(t5 - t4).count();

    cout << "Query preparation time (ms): " << prep_ms << endl;
    cout << "Server evaluation time (ms): " << eval_ms << endl;
    cout << "Result verification time (ms): " << verif_ms << endl;
    cout << "Membership result: " << (found ? "Found" : "Not Found") << endl;

    return 0;
}