/*
 * File: WordLadder.cpp
 * --------------------
 * Name: Chenglong Geng
 * This file is for the word ladder.
 */

#include "genlib.h"
#include "lexicon.h"
#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

void to_lower_letters(std::string &word);
int build_path(const std::string &start, const std::string &end, Lexicon &english_dict, std::unordered_map<std::string, std::set<std::string>> &path);
std::set<std::string> generate_successors(const std::string &cur, const std::string &end, Lexicon &english_dict, std::unordered_set<std::string> &words_visited);
void generate_ladder(std::unordered_map<std::string, std::set<std::string>> &path, int depth, std::set<std::vector<std::string>> &ladders, std::vector<std::string> ladder, const std::string &end);

int main() {
    std::cout << "Enter start word (RETURN to quit): ";
    std::string start_word;
    getline(std::cin, start_word);
    if(!start_word.empty()) {
        to_lower_letters(start_word);
        std::cout << "Enter destination word: ";
        std::string end_word;
        std::cin >> end_word;
        to_lower_letters(end_word);
        Lexicon english_dict("EnglishWords.dat");
        /*
         * if two words has a ladder
         * they must have the same length
         */
        if (start_word.length() != end_word.length())
            std::cout << "No ladder found." << std::endl;
        else if (start_word == end_word)
            std::cout << "Found ladder: " << start_word << " " << end_word <<std::endl;
        else {
            std::unordered_map<std::string, std::set<std::string>> path;
            int layer_nums = build_path(start_word, end_word, english_dict, path);
            std::set<std::vector<std::string>> ladders;
            std::vector<std::string> ladder;
            ladder.push_back(start_word);
            generate_ladder(path, layer_nums - 1, ladders, ladder, end_word);
            if(!ladders.empty()) {
                // output ladder(s)
                std::cout << "Found ladder: ";
                for(auto i = ladders.begin(); i != ladders.end(); i++) {
                    for(auto j = i->begin(); j != (i->end() - 1); j++)
                        std::cout << *j << " ";
                    std::cout << i->back() << std::endl;
                }
            } else
                std::cout << "No ladder found." << std::endl;
        }
    }
    return 0;
}

void to_lower_letters(std::string &word) {
    /*
     * transform upper letters into lower letters
     */
    std::transform(word.begin(), word.end(), word.begin(), (int(*)(int)) std::tolower);
}

std::set<std::string> generate_successors(const std::string &cur, const std::string &end, Lexicon &english_dict, std::unordered_set<std::string> &word_visited) {
    /*
     * generate successors of a word
     * try to enumerate replacing every letter in this word
     * check whether the generated word is in the dictionary
     * check whether the generated word has been visited before
     */
    std::set<std::string> successors;
	for(unsigned int i = 0; i < cur.length(); i++) {
        std::string word = cur;
		for(char c = 'a'; c <= 'z'; c++) {
            word[i] = c;
            if((english_dict.containsWord(word) && word_visited.count(word) != 1) || word == end)
                successors.insert(word);
		}
	}
    return successors;
}
int build_path(const std::string &start, const std::string &end, Lexicon &english_dict, std::unordered_map<std::string, std::set<std::string>> &path) {
    /*
     * initialise collection of visited words
     * collection of words on current layer and next layer
     * collection of paths
     * counter of layers
     */
    std::unordered_set<std::string> word_visited;
    std::set<std::string> cur_layer;
    std::set<std::string> next_layer;
    word_visited.insert(start);
    cur_layer.insert(start);
    int layer_counter = 0;
    /*
     * if current layer is empty
     * it means that no new word can be generated
     * if current layer contains end word
     * it means that the shortest path is ended on this layer
     */
    while(!cur_layer.empty() && cur_layer.count(end) != 1 ) {
        layer_counter += 1;
        /*
         * for every word on this layer
         * generate its successors which has not been visited before
         * record paths
         */
        for(auto i = cur_layer.begin(); i != cur_layer.end(); i++){
            std::set<std::string> tmp = generate_successors(*i, end, english_dict, word_visited);
            if(!tmp.empty()) {
                path.insert(std::pair<std::string, std::set<std::string>>(*i, tmp));
                next_layer.insert(tmp.begin(), tmp.end());
            }
        }
        /*
         * update collection of visited words
         * swap current layer and next layer
         */
        word_visited.insert(next_layer.begin(), next_layer.end());
        cur_layer.clear();
        cur_layer = next_layer;
        next_layer.clear();
    }
    if(cur_layer.empty())
        layer_counter -= 1;
    return layer_counter;
}

void generate_ladder(std::unordered_map<std::string, std::set<std::string>> &path, int depth, std::set<std::vector<std::string>> &ladders, std::vector<std::string> ladder, const std::string &end) {
    /*
     * if the depth of recursion is 0
     * that means the end of a ladder is reached
     * if the last word is the target
     * record this ladder into collection of ladders
     * if the depth of recursion is not 0
     * generate new ladders according to paths from this word
     */
    if(depth == 0) {
        if(path.count(ladder.back()))
            if(path[ladder.back()].count(end)) {
                ladder.push_back(end);
                ladders.insert(ladder);
            }
    }else {
        if(path.count(ladder.back())) {
            for(auto i = path[ladder.back()].begin(); i != path[ladder.back()].end(); i++) {
                std::vector<std::string> tmp = ladder;
                tmp.push_back(*i);
                generate_ladder(path, depth - 1, ladders, tmp, end);
            }
        }
    }
}
