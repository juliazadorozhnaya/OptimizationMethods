#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

int main() {
    int n, C;
    cin >> n >> C;

    vector<int> ws(n), ps(n);
    for (int i = 0; i < n; i++) {
        cin >> ws[i] >> ps[i];
    }

    vector<vector<tuple<int, int, int, int>>> g = {{{-1, -1, 0, 0}}};

    for (int i = 0; i < n; i++) {
        if (ws[i] > C) {
            continue;
        }
        vector<tuple<int, int, int, int>> new_states_0, new_states_1;
        for (auto& state : g.back()) {
            new_states_0.emplace_back(0, new_states_0.size(), get<2>(state), get<3>(state));
            if (get<2>(state) + ws[i] <= C) {
                new_states_1.emplace_back(1, new_states_1.size(), get<2>(state) + ws[i], get<3>(state) + ps[i]);
            }
        }
        vector<tuple<int, int, int, int>> new_states;
        int idx_0 = 0, idx_1 = 0;
        while (idx_0 < new_states_0.size() && idx_1 < new_states_1.size()) {
            if (get<2>(new_states_0[idx_0]) < get<2>(new_states_1[idx_1]) &&
                get<3>(new_states_0[idx_0]) < get<3>(new_states_1[idx_1])) {
                new_states.push_back(new_states_0[idx_0]);
                idx_0++;
            } else if (get<2>(new_states_0[idx_0]) > get<2>(new_states_1[idx_1]) &&
                    get<3>(new_states_0[idx_0]) > get<3>(new_states_1[idx_1])) {
                new_states.push_back(new_states_1[idx_1]);
                idx_1++;
            } else if (get<2>(new_states_0[idx_0]) <= get<2>(new_states_1[idx_1]) &&
                    get<3>(new_states_0[idx_0]) >= get<3>(new_states_1[idx_1])) {
                new_states.push_back(new_states_0[idx_0]);
                idx_0++;
                idx_1++;
            } else {
                new_states.push_back(new_states_1[idx_1]);
                idx_0++;
                idx_1++;
            }
        }
        while (idx_0 < new_states_0.size()) {
            new_states.push_back(new_states_0[idx_0]);
            idx_0++;
        }
        while (idx_1 < new_states_1.size()) {
            new_states.push_back(new_states_1[idx_1]);
            idx_1++;
        }
        // new_states.insert(new_states.end(), new_states_0.begin(), new_states_0.end());
        // new_states.insert(new_states.end(), new_states_1.begin(), new_states_1.end());
        g.push_back(new_states);
    }

    vector<int> selected;
    sort(g.back().begin(), g.back().end(), [](auto& x, auto& y) { return get<3>(x) < get<3>(y); });
    int record = get<3>(g.back().back());
    auto last_state = g.back().back();
    int i = g.size() - 2;
    while (i >= 0) {
        if (get<0>(last_state)) {
            selected.push_back(i);
        }
        last_state = g[i][get<1>(last_state)];
        i--;
    }

    cout << record << " " << selected.size() << endl;
    for (auto& i : selected) {
        cout << i << "\n";
    }   
}
