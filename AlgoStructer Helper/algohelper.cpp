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
#include <iomanip> // 출력 형식을 위한 라이브러리
#include <fstream> 
// 자료구조 정보를 저장하는 구조체 정의
// 각 자료구조에 대해 이름, 시간 복잡도, 설명을 포함함
struct DataStructureInfo {
    std::string name;         // 자료구조 이름
    std::string complexity;   // 시간 복잡도
    std::string description;  // 자료구조에 대한 설명
};

// 자료구조 정보 목록 (추천에 사용할 수 있는 자료구조와 그 특성)
const std::vector<DataStructureInfo> dataStructures = {
    {"스택", "O(1) 삽입/삭제", "LIFO 방식으로 데이터를 관리"},
    {"큐", "O(1) 삽입/삭제", "FIFO 방식으로 데이터를 관리"},
    {"이진 트리", "O(log n) 탐색/삽입/삭제", "정렬된 데이터를 관리하기에 적합"},
    {"링크드 리스트", "O(n) 접근, O(1) 삽입/삭제", "동적 데이터 관리에 적합"},
    {"어레이 리스트", "O(1) 접근, O(n) 삽입/삭제", "인덱스 기반 접근에 적합"},
    {"해시 맵", "O(1) 평균 접근, O(n) 최악의 경우", "빠른 검색에 적합"},
    {"해시 셋", "O(1) 평균 삽입/삭제", "중복 없는 데이터를 관리"}
};

// 코드 스니펫을 분석하여 적합한 자료구조를 추천하는 함수
// 이 함수는 키워드와 구조적 특징을 기반으로 자료구조를 추천함
void recommendDataStructure(const std::string& codeSnippet) {
    // 코드에 포함된 키워드와 해당 키워드에 적합한 자료구조를 매핑하는 테이블
    std::unordered_map<std::string, std::string> keywords = {
        {"push", "스택/큐"}, {"pop", "스택/큐"},
        {"insert", "어레이 리스트/링크드 리스트/해시 맵/이진 트리"},
        {"delete", "어레이 리스트/링크드 리스트/해시 맵/이진 트리"},
        {"find", "해시 맵/이진 트리"}, {"sort", "어레이 리스트/이진 트리"},
        {"access", "어레이 리스트/해시 맵"}, {"enqueue", "큐"},
        {"dequeue", "큐"}, {"stack", "스택"}, {"queue", "큐"},
        {"binary_tree", "이진 트리"}, {"linked_list", "링크드 리스트"},
        {"array", "어레이 리스트"}, {"hashmap", "해시 맵"},
        {"hashset", "해시 셋"}, {"unique", "해시 셋"},
        {"duplicate", "해시 맵/어레이 리스트"}, {"dynamic", "링크드 리스트"},
        {"sorted", "이진 트리"}
    };

    // 정규식을 사용하여 코드에서 반복문, 조건문, 배열 접근, 맵 접근 패턴을 찾음
    std::regex loopPattern("\\b(for|while)\\b");            // 반복문 패턴
    std::regex conditionPattern("\\b(if|else|switch)\\b");  // 조건문 패턴
    std::regex arrayAccessPattern("\\b[a-zA-Z_][a-zA-Z0-9_]*\\[[^\\]]+\\]"); // 배열 접근 패턴
    std::regex mapAccessPattern("\\b[a-zA-Z_][a-zA-Z0-9_]*\\.at\\(");        // 맵 접근 패턴

    // 반복문, 조건문, 배열 접근, 맵 접근 횟수 계산
    int loopCount = std::distance(std::sregex_iterator(codeSnippet.begin(), codeSnippet.end(), loopPattern), std::sregex_iterator());
    int conditionCount = std::distance(std::sregex_iterator(codeSnippet.begin(), codeSnippet.end(), conditionPattern), std::sregex_iterator());
    int arrayAccessCount = std::distance(std::sregex_iterator(codeSnippet.begin(), codeSnippet.end(), arrayAccessPattern), std::sregex_iterator());
    int mapAccessCount = std::distance(std::sregex_iterator(codeSnippet.begin(), codeSnippet.end(), mapAccessPattern), std::sregex_iterator());

    // 특정 키워드가 포함되었는지 여부를 판단하여 자료구조 추천 기준 강화
    bool dynamicData = codeSnippet.find("dynamic") != std::string::npos;  // 동적 데이터 사용 여부
    bool sortedData = codeSnippet.find("sorted") != std::string::npos;    // 정렬된 데이터 사용 여부
    bool uniqueData = codeSnippet.find("unique") != std::string::npos;    // 중복 없는 데이터 관리 여부
    bool duplicateData = codeSnippet.find("duplicate") != std::string::npos; // 중복 데이터 허용 여부

    // 코드에서 키워드를 추출하여 각 자료구조에 적합한 키워드 수를 카운트
    std::unordered_map<std::string, int> keywordCount;
    std::istringstream stream(codeSnippet);
    std::string word;
    while (stream >> word) {
        if (keywords.find(word) != keywords.end()) {
            keywordCount[keywords[word]]++;
        }
    }

    // 가장 적합한 자료구조를 찾기 위해 키워드 카운트 결과를 기반으로 비교
    std::pair<std::string, int> bestMatch = { "None", 0 };
    for (const auto& pair : keywordCount) {
        if (pair.second > bestMatch.second) {
            bestMatch = pair;
        }
    }

    // 분석 결과 출력
    std::cout << "코드 구조 분석:\n";
    std::cout << "- 반복문 수: " << loopCount << "\n";
    std::cout << "- 조건문 수: " << conditionCount << "\n";
    std::cout << "- 배열 접근 수: " << arrayAccessCount << "\n";
    std::cout << "- 맵 접근 수: " << mapAccessCount << "\n";
    std::cout << "- 동적 데이터 사용 여부: " << (dynamicData ? "예" : "아니오") << "\n";
    std::cout << "- 정렬된 데이터 사용 여부: " << (sortedData ? "예" : "아니오") << "\n";
    std::cout << "- 중복 없는 데이터 관리 여부: " << (uniqueData ? "예" : "아니오") << "\n";
    std::cout << "- 중복 데이터 허용 여부: " << (duplicateData ? "예" : "아니오") << "\n";

    // 추천 자료구조 출력
    std::cout << "\n추천 자료구조:\n";
    if (dynamicData) {
        std::cout << "- 링크드 리스트: 동적 데이터 관리에 적합\n";
    }
    if (sortedData) {
        std::cout << "- 이진 트리: 정렬된 데이터 관리에 적합\n";
    }
    if (uniqueData) {
        std::cout << "- 해시 셋: 중복 없는 데이터 관리에 적합\n";
    }
    if (duplicateData) {
        std::cout << "- 해시 맵/어레이 리스트: 중복 데이터 허용 관리에 적합\n";
    }

    // 추가 추천 자료구조 출력
    if (bestMatch.second > 0) {
        std::cout << "\n추가 추천 자료구조: " << bestMatch.first << std::endl;
        std::cout << "예상 시간 복잡도: " << (bestMatch.first == "스택" || bestMatch.first == "큐" ? "O(1)" : "O(log n) 또는 O(n)") << std::endl;
        std::cout << "\n성능 비교 (삽입, 삭제, 탐색):\n";
        std::cout << std::setw(15) << "자료구조" << std::setw(15) << "삽입" << std::setw(15) << "삭제" << std::setw(15) << "탐색" << std::endl;
        std::cout << std::setw(15) << "어레이 리스트" << std::setw(15) << "O(n)" << std::setw(15) << "O(n)" << std::setw(15) << "O(1)" << std::endl;
        std::cout << std::setw(15) << "링크드 리스트" << std::setw(15) << "O(1)" << std::setw(15) << "O(1)" << std::setw(15) << "O(n)" << std::endl;
        std::cout << std::setw(15) << "해시 맵" << std::setw(15) << "O(1)" << std::setw(15) << "O(1)" << std::setw(15) << "O(1)" << std::endl;
        std::cout << std::setw(15) << "이진 트리" << std::setw(15) << "O(log n)" << std::setw(15) << "O(log n)" << std::setw(15) << "O(log n)" << std::endl;
    }
    else {
        std::cout << "적합한 자료구조를 찾을 수 없습니다." << std::endl;
    }
}
// 선택 정렬 알고리즘
// 선택 정렬은 배열에서 가장 작은 값을 찾아 맨 앞으로 이동하는 방식으로 동작함
// 시간 복잡도: O(n^2)
void selectionSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx])
                minIdx = j;
        }
        std::swap(arr[i], arr[minIdx]); // 최소값을 맨 앞으로 이동
    }
}

// 삽입 정렬 알고리즘
// 삽입 정렬은 현재 요소를 적절한 위치에 삽입하여 정렬된 상태를 유지함
// 시간 복잡도: O(n^2)
void insertionSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];   // 현재 요소 저장
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j]; // 한 칸씩 오른쪽으로 이동
            j--;
        }
        arr[j + 1] = key; // 현재 요소를 적절한 위치에 삽입
    }
}

// 퀵 정렬 알고리즘
// 퀵 정렬은 분할 정복 방식을 사용하여 배열을 재귀적으로 정렬함
// 시간 복잡도: 평균 O(n log n), 최악 O(n^2)
void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high];  // 피벗 선택
        int i = low - 1;        // 작은 요소의 인덱스
        for (int j = low; j < high; j++) {
            if (arr[j] < pivot) {
                i++;
                std::swap(arr[i], arr[j]); // 피벗보다 작은 요소를 앞으로 이동
            }
        }
        std::swap(arr[i + 1], arr[high]); // 피벗을 올바른 위치에 삽입
        quickSort(arr, low, i);           // 왼쪽 부분 배열 정렬
        quickSort(arr, i + 2, high);      // 오른쪽 부분 배열 정렬
    }
}

// 순차 정렬 알고리즘 (STL의 정렬 함수 사용)
// std::sort() 함수는 C++ 표준 라이브러리에서 제공하는 효율적인 정렬 함수
// 시간 복잡도: O(n log n)
void sequentialSort(std::vector<int>& arr) {
    std::sort(arr.begin(), arr.end());
}

// 네 가지 정렬 알고리즘의 성능 및 결과를 비교하는 함수
void compareSortingAlgorithms(std::vector<int> arr) {
    std::cout << "정렬 알고리즘 비교:\n";
    std::vector<int> copy;
    int n = arr.size();  // 입력 데이터 크기
    const int repeatCount = 10;  // 각 정렬 알고리즘을 반복 실행할 횟수

    // 선택 정렬 수행 및 단계별 출력과 성능 측정
    std::cout << "============================================\n";
    std::cout << "선택 정렬 단계별 과정:\n";
    copy = arr;
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (copy[j] < copy[minIdx])
                minIdx = j;
        }
        std::swap(copy[i], copy[minIdx]);
        std::cout << "단계 " << i + 1 << ": ";
        for (const auto& val : copy) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
    std::cout << "선택 정렬 최종 결과: ";
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
    std::cout << "선택 정렬 평균 시간: " << (totalDuration / repeatCount) << " 나노초\n";
    std::cout << "선택 정렬 이론적인 시간 복잡도: " << n * n << " (최악 O(n^2) 연산 횟수 기준)\n";
    std::cout << "선택 정렬 공간 복잡도: 1 (상수 공간 사용)\n";
    std::cout << "============================================\n\n";

    // 삽입 정렬 수행 및 단계별 출력과 성능 측정
    std::cout << "============================================\n";
    std::cout << "삽입 정렬 단계별 과정:\n";
    copy = arr;
    for (int i = 1; i < n; i++) {
        int key = copy[i];
        int j = i - 1;
        while (j >= 0 && copy[j] > key) {
            copy[j + 1] = copy[j];
            j--;
        }
        copy[j + 1] = key;
        std::cout << "단계 " << i << ": ";
        for (const auto& val : copy) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
    std::cout << "삽입 정렬 최종 결과: ";
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
    std::cout << "삽입 정렬 평균 시간: " << (totalDuration / repeatCount) << " 나노초\n";
    std::cout << "삽입 정렬 이론적인 시간 복잡도: " << n * n << " (최악 O(n^2) 연산 횟수 기준)\n";
    std::cout << "삽입 정렬 공간 복잡도: 1 (상수 공간 사용)\n";
    std::cout << "============================================\n\n";

    // 퀵 정렬 수행 및 단계별 출력과 성능 측정
    std::cout << "============================================\n";
    std::cout << "퀵 정렬 단계별 과정:\n";
    copy = arr;
    quickSort(copy, 0, n - 1);
    std::cout << "퀵 정렬 최종 결과: ";
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
    std::cout << "퀵 정렬 평균 시간: " << (totalDuration / repeatCount) << " 나노초\n";
    std::cout << "퀵 정렬 이론적인 시간 복잡도: 평균 약 " << n * log2(n) << " (O(n log n) 연산 횟수 기준), 최악 약 " << n * n << " (O(n^2))\n";
    std::cout << "퀵 정렬 공간 복잡도: 약 " << log2(n) << " (재귀 호출 스택 공간)\n";
    std::cout << "============================================\n\n";

    // 순차 정렬(STL sort) 수행 및 단계별 출력과 성능 측정
    std::cout << "============================================\n";
    std::cout << "순차 정렬 단계별 과정:\n";
    copy = arr;
    sequentialSort(copy);
    std::cout << "순차 정렬 최종 결과: ";
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
    std::cout << "순차 정렬 평균 시간: " << (totalDuration / repeatCount) << " 나노초\n";
    std::cout << "순차 정렬 이론적인 시간 복잡도: 평균 약 " << n * log2(n) << " (O(n log n) 연산 횟수 기준)\n";
    std::cout << "순차 정렬 공간 복잡도: 1 (상수 공간 사용)\n";
    std::cout << "============================================\n";
}


// Graph algorithms

// 다익스트라 알고리즘: 최단 경로를 계산하는 알고리즘
// 시간 복잡도: O((V + E) log V)
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

    std::cout << "다익스트라 알고리즘 결과 (시작 정점: " << start << "):\n";
    for (int i = 0; i < n; i++) {
        std::cout << "정점 " << i << "까지의 거리: " << dist[i] << std::endl;
    }
}

// 프림 알고리즘: 최소 신장 트리를 계산하는 알고리즘
// 시간 복잡도: O((V + E) log V)
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

    std::cout << "프림 알고리즘 결과 (MST):\n";
    for (int i = 1; i < n; i++) {
        if (parent[i] != -1) {
            std::cout << "정점 " << parent[i] << " - " << i << " (가중치: " << key[i] << ")\n";
        }
    }
}

// 그래프 알고리즘 비교 함수: 사용자가 입력한 그래프에 대해 다익스트라와 프림 알고리즘을 실행
void compareGraphAlgorithms() {
    try {
        int n, m;
        std::cout << "정점 수 입력: ";
        if (!(std::cin >> n) || n <= 0) {
            throw std::invalid_argument("유효한 정점 수를 입력해야 합니다.");
        }

        std::cout << "간선 수 입력: ";
        if (!(std::cin >> m) || m < 0) {
            throw std::invalid_argument("유효한 간선 수를 입력해야 합니다.");
        }

        std::vector<std::vector<std::pair<int, int>>> graph(n);
        std::cout << "각 간선과 가중치 입력 (형식: u v w):\n";
        for (int i = 0; i < m; i++) {
            int u, v, w;
            if (!(std::cin >> u >> v >> w) || u < 0 || v < 0 || u >= n || v >= n || w < 0) {
                throw std::invalid_argument("유효한 정점 번호와 가중치를 입력해야 합니다.");
            }
            graph[u].emplace_back(v, w);
            graph[v].emplace_back(u, w);
        }

        int start;
        std::cout << "다익스트라 알고리즘 시작 정점 입력: ";
        if (!(std::cin >> start) || start < 0 || start >= n) {
            throw std::invalid_argument("유효한 시작 정점을 입력해야 합니다.");
        }

        dijkstraAlgorithm(graph, start);
        primAlgorithm(graph);
    } catch (const std::exception& e) {
        // 예외 발생 시 오류 메시지 출력 및 입력 스트림 상태 초기화
        std::cout << "오류: " << e.what() << " 다시 시도하세요.\n";
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
    }
}
int main() {
    int choice;  // 사용자 입력을 저장할 변수

    // 무한 루프를 사용하여 사용자가 종료를 선택할 때까지 반복
    while (true) {
        try {
            // 메뉴 출력
            std::cout << "메뉴:\n";
            std::cout << "1. 자료구조 추천\n";
            std::cout << "2. 정렬 알고리즘 비교\n";
            std::cout << "3. 그래프 알고리즘 비교\n";
            std::cout << "4. 종료\n";
            std::cout << "선택: ";

            // 사용자로부터 메뉴 선택을 입력받음
            if (!(std::cin >> choice)) {
                throw std::invalid_argument("숫자를 입력해야 합니다.");  // 숫자가 아닌 입력에 대한 예외 처리
            }

            if (choice == 4) break;  // 사용자가 4를 입력하면 프로그램 종료

            // 사용자의 선택에 따라 각각의 기능 수행
            switch (choice) {
            case 1: {
                // 1번: 자료구조 추천 기능
                std::cin.ignore();  // 입력 버퍼를 비움
                std::string input;
                std::cout << "코드 입력 (파일 경로 또는 여러 줄 코드 입력, 종료하려면 \"END\" 입력):\n";

                std::getline(std::cin, input);
                std::ifstream file(input);  // 파일 경로로 시도
                std::string codeSnippet;

                if (file) {
                    // 파일이 열리면 파일 내용을 읽어 코드로 사용
                    std::ostringstream buffer;
                    buffer << file.rdbuf();
                    codeSnippet = buffer.str();
                    std::cout << "파일에서 코드 로드 완료\n";
                }
                else {
                    // 파일이 없으면 여러 줄 입력받기 시작
                    std::cout << "파일이 존재하지 않음. 여러 줄 코드를 입력하세요 (\"END\" 입력 시 종료):\n";
                    std::ostringstream codeBuffer;
                    while (true) {
                        std::getline(std::cin, input);
                        if (input == "END") break;  // "END" 입력 시 종료
                        codeBuffer << input << '\n';
                    }
                    codeSnippet = codeBuffer.str();
                }

                recommendDataStructure(codeSnippet);  // 코드에 대해 자료구조 추천
                break;
            }
            case 2: {
                // 2번: 정렬 알고리즘 비교 기능
                std::vector<int> arr;  // 정수를 저장할 벡터
                int value;
                std::cout << "데이터 입력 (종료하려면 비정수를 입력): ";

                // 사용자가 정수 입력을 중단할 때까지 반복하여 벡터에 추가
                while (std::cin >> value) {
                    arr.push_back(value);
                }

                if (arr.empty()) {
                    // 사용자가 아무것도 입력하지 않았을 경우 예외 처리
                    throw std::invalid_argument("적어도 하나의 유효한 정수를 입력해야 합니다.");
                }

                std::cin.clear();                  // 입력 스트림 상태 초기화
                std::cin.ignore(INT_MAX, '\n');    // 남아 있는 입력 버퍼 비우기

                int n = arr.size();  // 입력받은 데이터의 크기
                std::cout << "\n입력된 데이터 크기: " << n << "\n";

                compareSortingAlgorithms(arr);     // 입력받은 배열에 대해 정렬 알고리즘 비교 수행
                break;
            }
            case 3:
                // 3번: 그래프 알고리즘 비교 기능
                compareGraphAlgorithms();
                break;
            default:
                // 잘못된 메뉴 선택에 대한 안내
                std::cout << "잘못된 입력입니다. 다시 시도하세요.\n";
            }
        }
        catch (const std::exception& e) {
            // 예외가 발생한 경우 오류 메시지 출력
            std::cout << "오류: " << e.what() << " 다시 시도하세요.\n";
            std::cin.clear();                  // 입력 스트림 상태 초기화
            std::cin.ignore(INT_MAX, '\n');    // 남아 있는 입력 버퍼 비우기
        }
    }

    return 0;  // 프로그램 종료
}
