#include <dpp/dpp.h>
#include <cctype>

using namespace std;

string getStringFromFile(const string& path) {
    ifstream inputStream;
    string token;

    try {
        inputStream.open(path);
        if (!inputStream)
            throw ifstream::failure("File \"" + path + "\" not found");

        getline(inputStream, token);
        if (inputStream.fail())
            throw ifstream::failure("An error occurred while reading the file");

        cout << "File read successful!\n";

        inputStream.close();
    }
    catch (const ifstream::failure& exception) {
        cout << "An exception occurred due to: " << exception.what() << "\n\n";

        if (inputStream.is_open())
            inputStream.close();

        throw;
    }

    return token;
}

string lowerString(string str) {
    string transformedStr = str;
    for (char& c : transformedStr) {
        c = std::tolower(static_cast<unsigned char>(c));
    }
    return transformedStr;
}

std::vector<std::string> splitIntoWords(const std::string& str) {
    istringstream iss(str);
    vector<std::string> words;
    string word;
    while (iss >> word) {
        words.push_back(word);
    }
    return words;
}

/// <summary>
/// Finds a specific Word inside of a discord message ignoring uppercase.
/// </summary>
/// <param name="event">Discord event</param>
/// <param name="word">Word to find in that message</param>
bool checkMessageContainsWord(const dpp::message_create_t& event, const std::string& word) {
    string message = event.msg.content;
    string lowerMessage = lowerString(message);
    string lowerWord = lowerString(word);

    vector<std::string> words = splitIntoWords(lowerMessage);

    return std::find(words.begin(), words.end(), lowerWord) != words.end();
}

/// <summary>
/// Finds a specific substring inside of discord message ignoring uppercase.
/// </summary>
/// <param name="event">Discord event</param>
/// <param name="word">String to find in that message</param>
bool checkMessageContainsString(const dpp::message_create_t& event, string word) {
    string message = event.msg.content;
    return lowerString(message).find(lowerString(word)) != std::string::npos;
}

int main() {
    string BOT_TOKEN = getStringFromFile("token.dat");

    dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);

    bot.on_log(dpp::utility::cout_logger());

    // Ping
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        }
    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id));
        }
    });

    // Respond
    bot.on_message_create([&bot](const dpp::message_create_t& event) {
        if (checkMessageContainsString(event, "kirigiri") || checkMessageContainsWord(event, "kiri") || checkMessageContainsString(event, "kyouko")) {
            event.reply("Did you mention me?", false);
        }
    });

    bot.start(dpp::st_wait);
    return 0;
}

/*
dpp::message msg(event.msg.channel_id, "I was mentioned");
bot.message_create(msg);
*/