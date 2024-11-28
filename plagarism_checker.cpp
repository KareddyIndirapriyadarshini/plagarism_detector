#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cmath>

using namespace std;

// preprocesses the text by converting to lowercase and removing punctuation
string preprocessText(const string& text) {
    string cleanedText;
    for (char c : text) {
        if (isalpha(c) || isspace(c)) { // keep only letters and spaces
            cleanedText += tolower(c);
        }
    }
    return cleanedText;
}

// splits the text into individual words
vector<string> splitIntoWords(const string& text) {
    vector<string> words;
    stringstream ss(text);
    string word;
    while (ss >> word) { // extract each word
        words.push_back(word);
    }
    return words;
}

// calculates jaccard similarity between two sets of words
double calculateJaccardSimilarity(const unordered_set<string>& set1, const unordered_set<string>& set2) {
    int intersectionCount = 0;
    int unionCount = set1.size() + set2.size();

    for (const auto& word : set1) {
        if (set2.count(word)) { // count common words
            intersectionCount++;
        }
    }

    unionCount -= intersectionCount; // adjust union count to avoid double counting
    return static_cast<double>(intersectionCount) / unionCount;
}

// calculates cosine similarity between two word frequency maps
double calculateCosineSimilarity(const unordered_map<string, int>& freq1, const unordered_map<string, int>& freq2) {
    double dotProduct = 0.0, magnitude1 = 0.0, magnitude2 = 0.0;

    for (const auto& pair : freq1) {
        if (freq2.count(pair.first)) {
            dotProduct += pair.second * freq2.at(pair.first); // calculate dot product
        }
    }

    for (const auto& pair : freq1) {
        magnitude1 += pair.second * pair.second; // calculate magnitude for doc1
    }

    for (const auto& pair : freq2) {
        magnitude2 += pair.second * pair.second; // calculate magnitude for doc2
    }

    return dotProduct / (sqrt(magnitude1) * sqrt(magnitude2));
}

// builds a frequency map of words in the document
unordered_map<string, int> buildFrequencyMap(const vector<string>& words) {
    unordered_map<string, int> freqMap;
    for (const auto& word : words) {
        freqMap[word]++; // count occurrences of each word
    }
    return freqMap;
}

// performs plagiarism check between two documents
void checkPlagiarism(const string& doc1, const string& doc2) {
    // preprocess both documents
    string processedDoc1 = preprocessText(doc1);
    string processedDoc2 = preprocessText(doc2);

    // split the documents into words
    vector<string> wordsDoc1 = splitIntoWords(processedDoc1);
    vector<string> wordsDoc2 = splitIntoWords(processedDoc2);

    // create sets of unique words for jaccard similarity
    unordered_set<string> setDoc1(wordsDoc1.begin(), wordsDoc1.end());
    unordered_set<string> setDoc2(wordsDoc2.begin(), wordsDoc2.end());

    // create frequency maps for cosine similarity
    unordered_map<string, int> freqMapDoc1 = buildFrequencyMap(wordsDoc1);
    unordered_map<string, int> freqMapDoc2 = buildFrequencyMap(wordsDoc2);

    // calculate jaccard similarity
    double jaccardSimilarity = calculateJaccardSimilarity(setDoc1, setDoc2);

    // calculate cosine similarity
    double cosineSimilarity = calculateCosineSimilarity(freqMapDoc1, freqMapDoc2);

    // display the results
    cout << "plagiarism detection results:" << endl;
    cout << "-----------------------------------" << endl;
    cout << "jaccard similarity (based on words): " << jaccardSimilarity * 100 << "%" << endl;
    cout << "cosine similarity (based on words): " << cosineSimilarity * 100 << "%" << endl;
    cout << "-----------------------------------" << endl;

    // issue a warning if either similarity score is too high
    if (jaccardSimilarity > 0.7 || cosineSimilarity > 0.7) {
        cout << "warning: high plagiarism detected!" << endl;
    } else {
        cout << "no significant plagiarism detected." << endl;
    }
}

// main function to take user input and run plagiarism check
int main() {
    string document1, document2;

    // get the first document from the user
    cout << "enter the first document: ";
    getline(cin, document1);

    // get the second document from the user
    cout << "enter the second document: ";
    getline(cin, document2);

    // check for plagiarism
    checkPlagiarism(document1, document2);

    return 0;
}
