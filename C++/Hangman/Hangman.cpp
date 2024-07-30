#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <random>
#include <queue>

using namespace std;

void cin_clean() // Completely clean cin of inputs
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.clear();
}

void DisplayGame(string message, bool display_top = true, bool display_bottom = true);
void UpdateHangman(int guess_amount = 0);
void DisplayLetters(string input, char start, char end);
void DisplayUnusedLetters(string used);
string GetRandWord(string filename, int minLength, int maxLength);
bool DisplayWord(string word, string guessed);
int TriesRemaining(string word, string guessed);
void DisplayLeaderboard(const string& filename);
void ClearLeaderboard(const string& filename);
void WriteTimeToLeaderboard(const string& filename, double time);

int main()
{
    char input = ' ';
    int minLength, maxLength;

    do
    {
        while (true) // Loop till correct input
        {
            system("cls");
            DisplayGame("Menu");
            DisplayGame("S - Start Hangman", false, false);
            DisplayGame("L - Leaderboard", false, false);
            DisplayGame("X - Exit Game", false, true);

            cout << endl << "Enter option: ";
            cin >> input;
            input = tolower(input);

            if (input != 's' && input != 'l' && input != 'x') // if invalid input
            {
                cin_clean();
                cout << endl << "Not a valid option, please try again.";
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
            else
            {
                cin_clean();
                break;
            }
        }

        switch (input)
        {
        case ('s'): // Start Game
        {
            char difficulty;

            while (true)
            {
                system("cls");

                DisplayGame("Difficulty Selection");
                DisplayGame("E - Easy", false, false);
                DisplayGame("M - Medium", false, false);
                DisplayGame("H - Hard", false, true);

                cout << endl << "Select difficulty: ";
                cin >> difficulty;
                difficulty = tolower(difficulty);


                if (difficulty == 'e') // Easy
                {
                    minLength = 3;
                    maxLength = 4;

                    cin_clean();
                    break;
                }
                else if (difficulty == 'm') // Medium
                {
                    minLength = 5;
                    maxLength = 6;

                    cin_clean();
                    break;
                }
                else if (difficulty == 'h') // Hard
                {
                    minLength = 7;
                    maxLength = numeric_limits<int>::max();

                    cin_clean();
                    break;
                }
                else // Invalid
                {
                    cout << endl << "Invalid difficulty..." << endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2));

                    cin_clean();
                }
            }

            string guesses = "";
            string guessWord;
            guessWord = GetRandWord("words.txt", minLength, maxLength);

            int tries = 0;
            bool win = false;

            auto start = chrono::high_resolution_clock::now(); // Start timer

            while (true) // Run Game
            {
                system("cls");
                DisplayGame("Hangman");
                UpdateHangman(tries);
                DisplayUnusedLetters(guesses);
                DisplayGame("Guess the word");
                win = DisplayWord(guessWord, guesses);

                if (win)
                {
                    break;
                }
                else if (tries > 7)
                {
                    break;
                }

                char input;
                cout << "Guess letter: ";
                cin >> input;
                input = tolower(input);

                if (guesses.find(input) == string::npos)
                {
                    guesses += input;
                }

                tries = TriesRemaining(guessWord, guesses);

            }

            auto end = chrono::high_resolution_clock::now(); // End timer
            chrono::duration<double> time_taken = end - start; // Calculate time taken

            if (win)
            {
                DisplayGame("You Won!!!");
                cout << endl << "Time taken: " << time_taken.count();
                WriteTimeToLeaderboard("leaderboard.txt", time_taken.count());
                cout << endl;
                system("pause");

                break;

            }
            else
            {
                system("cls");
                DisplayGame("Hangman");
                UpdateHangman(tries);

                DisplayGame("Game Over.");
                cout << endl << "The word was: \"" << guessWord << "\"." << endl << endl;
                system("pause");
                break;
            }
        }
        case 'l': // Leaderboard
        {
            system("cls");
            DisplayLeaderboard("leaderboard.txt");
            break;
        }
        case 'x': // Exit Game
            break;
        }

        system("cls");

    } while (input != 'x');
}

void DisplayGame(string message, bool display_top, bool display_bottom)
{
    if (display_top) // Top Border
    {
        cout << "*---------------------------------*" << endl;
        cout << "|";
    }
    else // Wall
    {
        cout << "|";
    }

    bool front = true;
    for (int i = message.length(); i < 33; i++)
    {
        if (front)
        {
            message = " " + message;
        }
        else
        {
            message = message + " ";
        }

        front = !front;
    }

    cout << message.c_str();

    if (display_bottom) // Bottom Border
    {
        cout << "|" << endl;
        cout << "*---------------------------------*" << endl;
    }
    else // Wall
    {
        cout << "|" << endl;
    }
}

void UpdateHangman(int guess_amount) // Used to display and update hangman
{
    if (guess_amount >= 1)
    {
        DisplayGame("|", false, false);
    }
    else
    {
        DisplayGame("", false, false);
    }

    if (guess_amount >= 2)
    {
        DisplayGame("|", false, false);
    }
    else
    {
        DisplayGame("", false, false);
    }

    if (guess_amount >= 3)
    {
        DisplayGame("O", false, false);
    }
    else
    {
        DisplayGame("", false, false);
    }

    if (guess_amount == 4)
    {
        DisplayGame("|", false, false);
    }

    if (guess_amount == 5)
    {
        DisplayGame("/| ", false, false);
    }

    if (guess_amount >= 6)
    {
        DisplayGame("/|\\", false, false);
    }
    else
    {
        DisplayGame("", false, false);
    }

    if (guess_amount == 7)
    {
        DisplayGame("/  ", false, false);
    }

    if (guess_amount >= 8)
    {
        DisplayGame("/ \\", false, false);
    }
    else
    {
        DisplayGame("", false, false);
    }
}

void DisplayLetters(string input, char start, char end)
{
    string str;
    for (char i = start; i <= end; i++)
    {
        if (input.find(i) == string::npos)
        {
            str += i;
            str += " ";
        }
        else
        {
            str += "  ";
        }
    }

    DisplayGame(str, false, false);
}

void DisplayUnusedLetters(string used)
{
    DisplayGame("Available letters");
    DisplayLetters(used, 'a', 'm');
    DisplayLetters(used, 'n', 'z');
}

string GetRandWord(string filename, int minLength, int maxLength) // Get random word from words.txt
{
    vector<string> vWords;
    string word;
    string lower_word;

    ifstream input_file;
    input_file.open(filename);

    if (input_file) // if file opened successfully
    {
        while (getline(input_file, word))
        {
            if (word.length() >= minLength && word.length() <= maxLength)
            {
                vWords.push_back(word);
            }
        }

        // Generate seed to select pseudo random word.
        unsigned seed = chrono::high_resolution_clock::now().time_since_epoch().count();
        mt19937 generator(seed);

        // Safety check to make sure vector isn't empty
        if (!vWords.empty())
        {
            uniform_int_distribution<int> distribution(0, vWords.size() - 1);
            int randline = distribution(generator);

            word = vWords.at(randline);
        }

        input_file.close();
    }

    // lowercase all characters in word
    for (char ch : word)
    {
        lower_word += tolower(ch);
    }
    return lower_word;
}

bool DisplayWord(string word, string guessed) // Displays word needed to guess and checks win condition
{
    bool won = true;
    string str;
    for (int i = 0; i < word.length(); i++)
    {
        if (guessed.find(word[i]) == string::npos)
        {
            won = false;
            str += "_ ";
        }
        else
        {
            str += word[i];
            str += " ";
        }
    }
    DisplayGame(str, false);

    return won;
}

int TriesRemaining(string word, string guessed) // Keeps track of incorrect guesses
{
    int incorrect = 0;

    for (int i = 0; i < guessed.length(); i++)
    {
        if (word.find(guessed[i]) == string::npos)
        {
            incorrect++;
        }
    }

    return incorrect;
}

void DisplayLeaderboard(const string& filename)
{
    ifstream input_file;
    input_file.open(filename);

    if (!input_file) // Create file if it doesn't exist
    {
        ofstream output_file(filename);
        output_file.close();
    }

    auto compare = [](const string& left, const string& right) {
        double left_time = stod(left.substr(0, right.find(" ")));
        double right_time = stod(right.substr(0, left.find(" ")));
        return left_time > right_time;
        };
    priority_queue<string, vector<string>, decltype(compare)> pq(compare);

    string line;
    while (getline(input_file, line))
    {
        pq.push(line);
        if (pq.size() > 10)
        {
            pq.pop();
        }
    }

    vector<string> top_times;
    while (!pq.empty())
    {
        top_times.push_back(pq.top());
        pq.pop();
    }

    DisplayGame("Leaderboard");

    for (const auto& time : top_times)
    {
        DisplayGame(time, false, false);
    }
    DisplayGame("", false, true); // Bottom border for leaderboard

    cout << endl << "Enter 'c' to clear leaderboard, \nor any other character to return to menu: ";
    char choice;
    cin >> choice;
    choice = tolower(choice);

    if (choice == 'c')
    {
        cout << endl << "Are you sure you want to clear the leaderboard? \nThis cannot be undone. (y/n): ";
        char confirm;
        cin >> confirm;
        confirm = tolower(confirm);

        if (confirm == 'y')
        {
            ClearLeaderboard(filename);
            cin_clean();
        }
        else
        {
            cin_clean();
        }
    }
    else
    {
        cin_clean();
    }
}

void ClearLeaderboard(const string& filename) // Clear Leaderboard.txt
{
    ofstream out_file(filename, ios::trunc);
    if (out_file.is_open())
    {
        out_file.close();
        cout << "Leaderboard cleared." << endl;
    }
    else
    {
        cout << "Unable to clear leaderboard file." << endl;
    }
}

void WriteTimeToLeaderboard(const string& filename, double time)
{
    ifstream input_file;
    input_file.open(filename);

    if (!input_file)
    {
        cout << "Unable to open leaderboard file." << endl;
        return;
    }

    vector<string> lines;
    string line;
    while (getline(input_file, line))
    {
        lines.push_back(line);
    }
    input_file.close();

    bool added = false;
    if (lines.size() < 10) // Check if new time should be added
    {
        added = true;
    }
    else
    {
        double worst_time = stod(lines.back().substr(0, lines.back().find(" ")));
        if (time < worst_time)
        {
            added = true;
        }
    }

    // Format the new time
    ostringstream oss;
    oss << fixed << setprecision(2) << time;
    string formatted_time = oss.str() + " seconds";

    if (added)
    {
        if (lines.size() == 10)
        {
            lines.pop_back(); // Remove the worst time
        }
        lines.push_back(formatted_time);
        sort(lines.begin(), lines.end(), [](const string& lhs, const string& rhs) {
            return stod(lhs.substr(0, lhs.find(" "))) < stod(rhs.substr(0, rhs.find(" ")));
            });
    }

    // Write updated leaderboard
    ofstream out_file(filename, ios::trunc);
    for (const auto& entry : lines)
    {
        out_file << entry << endl;
    }
    out_file.close();
}