#include <cctype>
#include <fstream>
#include <string>
#include <vector>

struct Entry {
  Entry(std::string &&enum_, std::string &&string) {
    enum_representation = std::move(enum_);
    string_representation = std::move(string);

    if (string_representation.front() == '!') {
      if (string_representation.at(1) == 'B')
        command_type = BEGIN;
      else
        command_type = END;
    } else
      command_type = NOT_COMMAND;
  }

  std::string enum_representation;
  std::string string_representation;

  enum { NOT_COMMAND, BEGIN, END } command_type;
};

static std::ifstream input_file;
static std::ofstream output_file;
static std::vector<Entry> entries;

static std::string enum_name;
static std::string underlying_type;

static void parseCommand(std::string &command) {
  if (command.at(1) == 'F')
    return;

  command.push_back('_');

  static std::string next;
  input_file >> next;
  command.append(next);

  next = command.substr(1);
  entries.emplace_back(std::move(next), std::move(command));
}

static void parseLoop() {

  while (!input_file.eof()) {
    static std::string next;
    input_file >> next;
    if (next.front() == '!') {
      parseCommand(next);
      continue;
    }

    static std::string string_rep;
    input_file >> string_rep;

    entries.emplace_back(std::move(next), std::move(string_rep));
  }
}

static void printDefinition() {
  output_file << "#pragma once\n"
              << "#include <unordered_map>\n"
              << "#include <string>\n"
              << "#include <utility>\n";

  output_file << "enum class " << enum_name << " : " << underlying_type
              << " {\n\n";

  auto curr = entries.cbegin();
  const auto end = entries.cend();

  while (curr != end) {
    output_file << "  " << curr->enum_representation << ",\n";
    if (curr->command_type == Entry::END)
      output_file << '\n';
    ++curr;
  }

  output_file << "};\n\n\n";
}

static void printMapping() {
  output_file << "inline const std::unordered_map<std::string, " << enum_name
              << "> stringTo" << enum_name << "{\n";

  auto curr = entries.cbegin();
  const auto end = entries.cend();
  static constexpr unsigned num_per_row{3};
  unsigned i{};
  while (curr != end) {
    bool newline = (i % num_per_row == 0);

    if (curr->command_type == Entry::NOT_COMMAND) {
      if (newline)
        output_file << "\n\t";

      ++i;
    } else {
      if (curr->command_type == Entry::BEGIN)
        output_file << '\n';
      output_file << "\n\t";
      i = 0;
    }

    output_file << "{\"" << curr->string_representation << "\", " << enum_name
                << "::" << curr->enum_representation << "}, ";

    if (curr->command_type == Entry::END)
      output_file << "\n";

    ++curr;
  }

  output_file << "\n};\n\n\n";
}

static void printArray() {
  std::string enumFirstLowercase = enum_name;
  enumFirstLowercase.front() = std::tolower(enumFirstLowercase.front());

  output_file << "constexpr const char* " << enumFirstLowercase << "ToString("
              << enum_name << " e) {\n";
  output_file << "constexpr const char *" << "toString[] = {\n";

  auto curr = entries.cbegin();
  const auto end = entries.cend();
  static constexpr unsigned num_per_row{3};
  unsigned i{};
  while (curr != end) {
    bool newline = (i % num_per_row == 0);

    if (curr->command_type == Entry::NOT_COMMAND) {
      if (newline)
        output_file << "\n\t";

      ++i;
    } else {
      if (curr->command_type == Entry::BEGIN)
        output_file << '\n';
      output_file << "\n\t";
      i = 0;
    }

    output_file << "\"" << curr->string_representation << "\",";

    if (curr->command_type == Entry::END)
      output_file << "\n";

    ++curr;
  }

  output_file << "\n}; \nreturn toString[std::to_underlying(e)];\n}\n";
}

int main(int argc, char *argv[]) {
  if (argc != 2)
    throw std::runtime_error(
        "One argument only, must be path to the input file");

  entries.reserve(30);
  input_file.open(argv[1]);
  if (!input_file.is_open())
    throw std::runtime_error("File not found");

  input_file >> std::ws;
  input_file >> enum_name;
  input_file >> underlying_type;

  parseLoop();
  input_file.close();

  std::string output_file_name(enum_name);
  output_file_name.append("_generated.hpp");
  output_file.open(output_file_name);
  if (!output_file.is_open())
    throw std::runtime_error("Error creating enum generation file");

  printDefinition();
  printMapping();
  printArray();

  return 0;
}
