#include <iostream>

using namespace std;

static string V = "CJ";
static string U = "Bi";
static int k;
static string ans;

void dfs(int v) {
    if (v == 1000) {
        cout << ans << endl;
        k--;
        if (k == 0) {
            exit(0);
        }
    } else {
        ans += V;
        dfs(v + 2);
        ans.erase(ans.begin() + v, ans.end());
        ans += U;
        dfs(v + 2);
        ans.erase(ans.begin() + v, ans.end());
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin >> k;
    dfs(0);
    return 0;
}
