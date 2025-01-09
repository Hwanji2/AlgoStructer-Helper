#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <climits>
#include <stdexcept>
#include <chrono>
#include <queue>
#include <regex>
#include <iomanip> // ��� ������ ���� ���̺귯��
#include <fstream> 
// �ڷᱸ�� ������ �����ϴ� ����ü ����
// �� �ڷᱸ���� ���� �̸�, �ð� ���⵵, ������ ������
struct DataStructureInfo {
    std::string name;         // �ڷᱸ�� �̸�
    std::string complexity;   // �ð� ���⵵
    std::string description;  // �ڷᱸ���� ���� ����
};

// �ڷᱸ�� ���� ��� (��õ�� ����� �� �ִ� �ڷᱸ���� �� Ư��)
const std::vector<DataStructureInfo> dataStructures = {
    {"����", "O(1) ����/����", "LIFO ������� �����͸� ����"},
    {"ť", "O(1) ����/����", "FIFO ������� �����͸� ����"},
    {"���� Ʈ��", "O(log n) Ž��/����/����", "���ĵ� �����͸� �����ϱ⿡ ����"},
    {"��ũ�� ����Ʈ", "O(n) ����, O(1) ����/����", "���� ������ ������ ����"},
    {"��� ����Ʈ", "O(1) ����, O(n) ����/����", "�ε��� ��� ���ٿ� ����"},
    {"�ؽ� ��", "O(1) ��� ����, O(n) �־��� ���", "���� �˻��� ����"},
    {"�ؽ� ��", "O(1) ��� ����/����", "�ߺ� ���� �����͸� ����"}
};

// �ڵ� �������� �м��Ͽ� ������ �ڷᱸ���� ��õ�ϴ� �Լ�
// �� �Լ��� Ű����� ������ Ư¡�� ������� �ڷᱸ���� ��õ��
void recommendDataStructure(const std::string& codeSnippet) {
    // �ڵ忡 ���Ե� Ű����� �ش� Ű���忡 ������ �ڷᱸ���� �����ϴ� ���̺�
    std::unordered_map<std::string, std::string> keywords = {
        {"push", "����/ť"}, {"pop", "����/ť"},
        {"insert", "��� ����Ʈ/��ũ�� ����Ʈ/�ؽ� ��/���� Ʈ��"},
        {"delete", "��� ����Ʈ/��ũ�� ����Ʈ/�ؽ� ��/���� Ʈ��"},
        {"find", "�ؽ� ��/���� Ʈ��"}, {"sort", "��� ����Ʈ/���� Ʈ��"},
        {"access", "��� ����Ʈ/�ؽ� ��"}, {"enqueue", "ť"},
        {"dequeue", "ť"}, {"stack", "����"}, {"queue", "ť"},
        {"binary_tree", "���� Ʈ��"}, {"linked_list", "��ũ�� ����Ʈ"},
        {"array", "��� ����Ʈ"}, {"hashmap", "�ؽ� ��"},
        {"hashset", "�ؽ� ��"}, {"unique", "�ؽ� ��"},
        {"duplicate", "�ؽ� ��/��� ����Ʈ"}, {"dynamic", "��ũ�� ����Ʈ"},
        {"sorted", "���� Ʈ��"}
    };

    // ���Խ��� ����Ͽ� �ڵ忡�� �ݺ���, ���ǹ�, �迭 ����, �� ���� ������ ã��
    std::regex loopPattern("\\b(for|while)\\b");            // �ݺ��� ����
    std::regex conditionPattern("\\b(if|else|switch)\\b");  // ���ǹ� ����
    std::regex arrayAccessPattern("\\b[a-zA-Z_][a-zA-Z0-9_]*\\[[^\\]]+\\]"); // �迭 ���� ����
    std::regex mapAccessPattern("\\b[a-zA-Z_][a-zA-Z0-9_]*\\.at\\(");        // �� ���� ����

    // �ݺ���, ���ǹ�, �迭 ����, �� ���� Ƚ�� ���
    int loopCount = std::distance(std::sregex_iterator(codeSnippet.begin(), codeSnippet.end(), loopPattern), std::sregex_iterator());
    int conditionCount = std::distance(std::sregex_iterator(codeSnippet.begin(), codeSnippet.end(), conditionPattern), std::sregex_iterator());
    int arrayAccessCount = std::distance(std::sregex_iterator(codeSnippet.begin(), codeSnippet.end(), arrayAccessPattern), std::sregex_iterator());
    int mapAccessCount = std::distance(std::sregex_iterator(codeSnippet.begin(), codeSnippet.end(), mapAccessPattern), std::sregex_iterator());

    // Ư�� Ű���尡 ���ԵǾ����� ���θ� �Ǵ��Ͽ� �ڷᱸ�� ��õ ���� ��ȭ
    bool dynamicData = codeSnippet.find("dynamic") != std::string::npos;  // ���� ������ ��� ����
    bool sortedData = codeSnippet.find("sorted") != std::string::npos;    // ���ĵ� ������ ��� ����
    bool uniqueData = codeSnippet.find("unique") != std::string::npos;    // �ߺ� ���� ������ ���� ����
    bool duplicateData = codeSnippet.find("duplicate") != std::string::npos; // �ߺ� ������ ��� ����

    // �ڵ忡�� Ű���带 �����Ͽ� �� �ڷᱸ���� ������ Ű���� ���� ī��Ʈ
    std::unordered_map<std::string, int> keywordCount;
    std::istringstream stream(codeSnippet);
    std::string word;
    while (stream >> word) {
        if (keywords.find(word) != keywords.end()) {
            keywordCount[keywords[word]]++;
        }
    }

    // ���� ������ �ڷᱸ���� ã�� ���� Ű���� ī��Ʈ ����� ������� ��
    std::pair<std::string, int> bestMatch = { "None", 0 };
    for (const auto& pair : keywordCount) {
        if (pair.second > bestMatch.second) {
            bestMatch = pair;
        }
    }

    // �м� ��� ���
    std::cout << "�ڵ� ���� �м�:\n";
    std::cout << "- �ݺ��� ��: " << loopCount << "\n";
    std::cout << "- ���ǹ� ��: " << conditionCount << "\n";
    std::cout << "- �迭 ���� ��: " << arrayAccessCount << "\n";
    std::cout << "- �� ���� ��: " << mapAccessCount << "\n";
    std::cout << "- ���� ������ ��� ����: " << (dynamicData ? "��" : "�ƴϿ�") << "\n";
    std::cout << "- ���ĵ� ������ ��� ����: " << (sortedData ? "��" : "�ƴϿ�") << "\n";
    std::cout << "- �ߺ� ���� ������ ���� ����: " << (uniqueData ? "��" : "�ƴϿ�") << "\n";
    std::cout << "- �ߺ� ������ ��� ����: " << (duplicateData ? "��" : "�ƴϿ�") << "\n";

    // ��õ �ڷᱸ�� ���
    std::cout << "\n��õ �ڷᱸ��:\n";
    if (dynamicData) {
        std::cout << "- ��ũ�� ����Ʈ: ���� ������ ������ ����\n";
    }
    if (sortedData) {
        std::cout << "- ���� Ʈ��: ���ĵ� ������ ������ ����\n";
    }
    if (uniqueData) {
        std::cout << "- �ؽ� ��: �ߺ� ���� ������ ������ ����\n";
    }
    if (duplicateData) {
        std::cout << "- �ؽ� ��/��� ����Ʈ: �ߺ� ������ ��� ������ ����\n";
    }

    // �߰� ��õ �ڷᱸ�� ���
    if (bestMatch.second > 0) {
        std::cout << "\n�߰� ��õ �ڷᱸ��: " << bestMatch.first << std::endl;
        std::cout << "���� �ð� ���⵵: " << (bestMatch.first == "����" || bestMatch.first == "ť" ? "O(1)" : "O(log n) �Ǵ� O(n)") << std::endl;
        std::cout << "\n���� �� (����, ����, Ž��):\n";
        std::cout << std::setw(15) << "�ڷᱸ��" << std::setw(15) << "����" << std::setw(15) << "����" << std::setw(15) << "Ž��" << std::endl;
        std::cout << std::setw(15) << "��� ����Ʈ" << std::setw(15) << "O(n)" << std::setw(15) << "O(n)" << std::setw(15) << "O(1)" << std::endl;
        std::cout << std::setw(15) << "��ũ�� ����Ʈ" << std::setw(15) << "O(1)" << std::setw(15) << "O(1)" << std::setw(15) << "O(n)" << std::endl;
        std::cout << std::setw(15) << "�ؽ� ��" << std::setw(15) << "O(1)" << std::setw(15) << "O(1)" << std::setw(15) << "O(1)" << std::endl;
        std::cout << std::setw(15) << "���� Ʈ��" << std::setw(15) << "O(log n)" << std::setw(15) << "O(log n)" << std::setw(15) << "O(log n)" << std::endl;
    }
    else {
        std::cout << "������ �ڷᱸ���� ã�� �� �����ϴ�." << std::endl;
    }
}
// ���� ���� �˰���
// ���� ������ �迭���� ���� ���� ���� ã�� �� ������ �̵��ϴ� ������� ������
// �ð� ���⵵: O(n^2)
void selectionSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx])
                minIdx = j;
        }
        std::swap(arr[i], arr[minIdx]); // �ּҰ��� �� ������ �̵�
    }
}

// ���� ���� �˰���
// ���� ������ ���� ��Ҹ� ������ ��ġ�� �����Ͽ� ���ĵ� ���¸� ������
// �ð� ���⵵: O(n^2)
void insertionSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];   // ���� ��� ����
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j]; // �� ĭ�� ���������� �̵�
            j--;
        }
        arr[j + 1] = key; // ���� ��Ҹ� ������ ��ġ�� ����
    }
}

// �� ���� �˰���
// �� ������ ���� ���� ����� ����Ͽ� �迭�� ��������� ������
// �ð� ���⵵: ��� O(n log n), �־� O(n^2)
void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high];  // �ǹ� ����
        int i = low - 1;        // ���� ����� �ε���
        for (int j = low; j < high; j++) {
            if (arr[j] < pivot) {
                i++;
                std::swap(arr[i], arr[j]); // �ǹ����� ���� ��Ҹ� ������ �̵�
            }
        }
        std::swap(arr[i + 1], arr[high]); // �ǹ��� �ùٸ� ��ġ�� ����
        quickSort(arr, low, i);           // ���� �κ� �迭 ����
        quickSort(arr, i + 2, high);      // ������ �κ� �迭 ����
    }
}

// ���� ���� �˰��� (STL�� ���� �Լ� ���)
// std::sort() �Լ��� C++ ǥ�� ���̺귯������ �����ϴ� ȿ������ ���� �Լ�
// �ð� ���⵵: O(n log n)
void sequentialSort(std::vector<int>& arr) {
    std::sort(arr.begin(), arr.end());
}

// �� ���� ���� �˰����� ���� �� ����� ���ϴ� �Լ�
void compareSortingAlgorithms(std::vector<int> arr) {
    std::cout << "���� �˰��� ��:\n";
    std::vector<int> copy;
    int n = arr.size();  // �Է� ������ ũ��
    const int repeatCount = 10;  // �� ���� �˰����� �ݺ� ������ Ƚ��

    // ���� ���� ���� �� �ܰ躰 ��°� ���� ����
    std::cout << "============================================\n";
    std::cout << "���� ���� �ܰ躰 ����:\n";
    copy = arr;
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (copy[j] < copy[minIdx])
                minIdx = j;
        }
        std::swap(copy[i], copy[minIdx]);
        std::cout << "�ܰ� " << i + 1 << ": ";
        for (const auto& val : copy) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
    std::cout << "���� ���� ���� ���: ";
    for (const auto& val : copy) {
        std::cout << val << " ";
    }
    std::cout << "\n\n";

    long long totalDuration = 0;
    for (int i = 0; i < repeatCount; i++) {
        copy = arr;
        auto start = std::chrono::high_resolution_clock::now();
        selectionSort(copy);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        totalDuration += duration;
    }
    std::cout << "���� ���� ��� �ð�: " << (totalDuration / repeatCount) << " ������\n";
    std::cout << "���� ���� �̷����� �ð� ���⵵: " << n * n << " (�־� O(n^2) ���� Ƚ�� ����)\n";
    std::cout << "���� ���� ���� ���⵵: 1 (��� ���� ���)\n";
    std::cout << "============================================\n\n";

    // ���� ���� ���� �� �ܰ躰 ��°� ���� ����
    std::cout << "============================================\n";
    std::cout << "���� ���� �ܰ躰 ����:\n";
    copy = arr;
    for (int i = 1; i < n; i++) {
        int key = copy[i];
        int j = i - 1;
        while (j >= 0 && copy[j] > key) {
            copy[j + 1] = copy[j];
            j--;
        }
        copy[j + 1] = key;
        std::cout << "�ܰ� " << i << ": ";
        for (const auto& val : copy) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
    std::cout << "���� ���� ���� ���: ";
    for (const auto& val : copy) {
        std::cout << val << " ";
    }
    std::cout << "\n\n";

    totalDuration = 0;
    for (int i = 0; i < repeatCount; i++) {
        copy = arr;
        auto start = std::chrono::high_resolution_clock::now();
        insertionSort(copy);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        totalDuration += duration;
    }
    std::cout << "���� ���� ��� �ð�: " << (totalDuration / repeatCount) << " ������\n";
    std::cout << "���� ���� �̷����� �ð� ���⵵: " << n * n << " (�־� O(n^2) ���� Ƚ�� ����)\n";
    std::cout << "���� ���� ���� ���⵵: 1 (��� ���� ���)\n";
    std::cout << "============================================\n\n";

    // �� ���� ���� �� �ܰ躰 ��°� ���� ����
    std::cout << "============================================\n";
    std::cout << "�� ���� �ܰ躰 ����:\n";
    copy = arr;
    quickSort(copy, 0, n - 1);
    std::cout << "�� ���� ���� ���: ";
    for (const auto& val : copy) {
        std::cout << val << " ";
    }
    std::cout << "\n\n";

    totalDuration = 0;
    for (int i = 0; i < repeatCount; i++) {
        copy = arr;
        auto start = std::chrono::high_resolution_clock::now();
        quickSort(copy, 0, n - 1);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        totalDuration += duration;
    }
    std::cout << "�� ���� ��� �ð�: " << (totalDuration / repeatCount) << " ������\n";
    std::cout << "�� ���� �̷����� �ð� ���⵵: ��� �� " << n * log2(n) << " (O(n log n) ���� Ƚ�� ����), �־� �� " << n * n << " (O(n^2))\n";
    std::cout << "�� ���� ���� ���⵵: �� " << log2(n) << " (��� ȣ�� ���� ����)\n";
    std::cout << "============================================\n\n";

    // ���� ����(STL sort) ���� �� �ܰ躰 ��°� ���� ����
    std::cout << "============================================\n";
    std::cout << "���� ���� �ܰ躰 ����:\n";
    copy = arr;
    sequentialSort(copy);
    std::cout << "���� ���� ���� ���: ";
    for (const auto& val : copy) {
        std::cout << val << " ";
    }
    std::cout << "\n\n";

    totalDuration = 0;
    for (int i = 0; i < repeatCount; i++) {
        copy = arr;
        auto start = std::chrono::high_resolution_clock::now();
        sequentialSort(copy);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        totalDuration += duration;
    }
    std::cout << "���� ���� ��� �ð�: " << (totalDuration / repeatCount) << " ������\n";
    std::cout << "���� ���� �̷����� �ð� ���⵵: ��� �� " << n * log2(n) << " (O(n log n) ���� Ƚ�� ����)\n";
    std::cout << "���� ���� ���� ���⵵: 1 (��� ���� ���)\n";
    std::cout << "============================================\n";
}


// Graph algorithms

// ���ͽ�Ʈ�� �˰���: �ִ� ��θ� ����ϴ� �˰���
// �ð� ���⵵: O((V + E) log V)
void dijkstraAlgorithm(const std::vector<std::vector<std::pair<int, int>>>& graph, int start) {
    int n = graph.size();
    std::vector<int> dist(n, INT_MAX);
    dist[start] = 0;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;

        for (const auto& edge : graph[u]) {
            int v = edge.first, weight = edge.second;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    std::cout << "���ͽ�Ʈ�� �˰��� ��� (���� ����: " << start << "):\n";
    for (int i = 0; i < n; i++) {
        std::cout << "���� " << i << "������ �Ÿ�: " << dist[i] << std::endl;
    }
}

// ���� �˰���: �ּ� ���� Ʈ���� ����ϴ� �˰���
// �ð� ���⵵: O((V + E) log V)
void primAlgorithm(const std::vector<std::vector<std::pair<int, int>>>& graph) {
    int n = graph.size();
    std::vector<int> key(n, INT_MAX);
    std::vector<bool> inMST(n, false);
    std::vector<int> parent(n, -1);
    key[0] = 0;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    pq.push({0, 0});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        inMST[u] = true;

        for (const auto& edge : graph[u]) {
            int v = edge.first, weight = edge.second;
            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                pq.push({key[v], v});
                parent[v] = u;
            }
        }
    }

    std::cout << "���� �˰��� ��� (MST):\n";
    for (int i = 1; i < n; i++) {
        if (parent[i] != -1) {
            std::cout << "���� " << parent[i] << " - " << i << " (����ġ: " << key[i] << ")\n";
        }
    }
}

// �׷��� �˰��� �� �Լ�: ����ڰ� �Է��� �׷����� ���� ���ͽ�Ʈ��� ���� �˰����� ����
void compareGraphAlgorithms() {
    try {
        int n, m;
        std::cout << "���� �� �Է�: ";
        if (!(std::cin >> n) || n <= 0) {
            throw std::invalid_argument("��ȿ�� ���� ���� �Է��ؾ� �մϴ�.");
        }

        std::cout << "���� �� �Է�: ";
        if (!(std::cin >> m) || m < 0) {
            throw std::invalid_argument("��ȿ�� ���� ���� �Է��ؾ� �մϴ�.");
        }

        std::vector<std::vector<std::pair<int, int>>> graph(n);
        std::cout << "�� ������ ����ġ �Է� (����: u v w):\n";
        for (int i = 0; i < m; i++) {
            int u, v, w;
            if (!(std::cin >> u >> v >> w) || u < 0 || v < 0 || u >= n || v >= n || w < 0) {
                throw std::invalid_argument("��ȿ�� ���� ��ȣ�� ����ġ�� �Է��ؾ� �մϴ�.");
            }
            graph[u].emplace_back(v, w);
            graph[v].emplace_back(u, w);
        }

        int start;
        std::cout << "���ͽ�Ʈ�� �˰��� ���� ���� �Է�: ";
        if (!(std::cin >> start) || start < 0 || start >= n) {
            throw std::invalid_argument("��ȿ�� ���� ������ �Է��ؾ� �մϴ�.");
        }

        dijkstraAlgorithm(graph, start);
        primAlgorithm(graph);
    } catch (const std::exception& e) {
        // ���� �߻� �� ���� �޽��� ��� �� �Է� ��Ʈ�� ���� �ʱ�ȭ
        std::cout << "����: " << e.what() << " �ٽ� �õ��ϼ���.\n";
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
    }
}
int main() {
    int choice;  // ����� �Է��� ������ ����

    // ���� ������ ����Ͽ� ����ڰ� ���Ḧ ������ ������ �ݺ�
    while (true) {
        try {
            // �޴� ���
            std::cout << "�޴�:\n";
            std::cout << "1. �ڷᱸ�� ��õ\n";
            std::cout << "2. ���� �˰��� ��\n";
            std::cout << "3. �׷��� �˰��� ��\n";
            std::cout << "4. ����\n";
            std::cout << "����: ";

            // ����ڷκ��� �޴� ������ �Է¹���
            if (!(std::cin >> choice)) {
                throw std::invalid_argument("���ڸ� �Է��ؾ� �մϴ�.");  // ���ڰ� �ƴ� �Է¿� ���� ���� ó��
            }

            if (choice == 4) break;  // ����ڰ� 4�� �Է��ϸ� ���α׷� ����

            // ������� ���ÿ� ���� ������ ��� ����
            switch (choice) {
            case 1: {
                // 1��: �ڷᱸ�� ��õ ���
                std::cin.ignore();  // �Է� ���۸� ���
                std::string input;
                std::cout << "�ڵ� �Է� (���� ��� �Ǵ� ���� �� �ڵ� �Է�, �����Ϸ��� \"END\" �Է�):\n";

                std::getline(std::cin, input);
                std::ifstream file(input);  // ���� ��η� �õ�
                std::string codeSnippet;

                if (file) {
                    // ������ ������ ���� ������ �о� �ڵ�� ���
                    std::ostringstream buffer;
                    buffer << file.rdbuf();
                    codeSnippet = buffer.str();
                    std::cout << "���Ͽ��� �ڵ� �ε� �Ϸ�\n";
                }
                else {
                    // ������ ������ ���� �� �Է¹ޱ� ����
                    std::cout << "������ �������� ����. ���� �� �ڵ带 �Է��ϼ��� (\"END\" �Է� �� ����):\n";
                    std::ostringstream codeBuffer;
                    while (true) {
                        std::getline(std::cin, input);
                        if (input == "END") break;  // "END" �Է� �� ����
                        codeBuffer << input << '\n';
                    }
                    codeSnippet = codeBuffer.str();
                }

                recommendDataStructure(codeSnippet);  // �ڵ忡 ���� �ڷᱸ�� ��õ
                break;
            }
            case 2: {
                // 2��: ���� �˰��� �� ���
                std::vector<int> arr;  // ������ ������ ����
                int value;
                std::cout << "������ �Է� (�����Ϸ��� �������� �Է�): ";

                // ����ڰ� ���� �Է��� �ߴ��� ������ �ݺ��Ͽ� ���Ϳ� �߰�
                while (std::cin >> value) {
                    arr.push_back(value);
                }

                if (arr.empty()) {
                    // ����ڰ� �ƹ��͵� �Է����� �ʾ��� ��� ���� ó��
                    throw std::invalid_argument("��� �ϳ��� ��ȿ�� ������ �Է��ؾ� �մϴ�.");
                }

                std::cin.clear();                  // �Է� ��Ʈ�� ���� �ʱ�ȭ
                std::cin.ignore(INT_MAX, '\n');    // ���� �ִ� �Է� ���� ����

                int n = arr.size();  // �Է¹��� �������� ũ��
                std::cout << "\n�Էµ� ������ ũ��: " << n << "\n";

                compareSortingAlgorithms(arr);     // �Է¹��� �迭�� ���� ���� �˰��� �� ����
                break;
            }
            case 3:
                // 3��: �׷��� �˰��� �� ���
                compareGraphAlgorithms();
                break;
            default:
                // �߸��� �޴� ���ÿ� ���� �ȳ�
                std::cout << "�߸��� �Է��Դϴ�. �ٽ� �õ��ϼ���.\n";
            }
        }
        catch (const std::exception& e) {
            // ���ܰ� �߻��� ��� ���� �޽��� ���
            std::cout << "����: " << e.what() << " �ٽ� �õ��ϼ���.\n";
            std::cin.clear();                  // �Է� ��Ʈ�� ���� �ʱ�ȭ
            std::cin.ignore(INT_MAX, '\n');    // ���� �ִ� �Է� ���� ����
        }
    }

    return 0;  // ���α׷� ����
}
