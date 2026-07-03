import sys

class Category:
    name: str
    begin: int
    end: int
    def __init__(self, name: str, begin: int, end: int):
        self.name = name
        self.begin = begin
        self.end = end

class Entry:
    enum_rep: str
    string_rep: str
    def __init__(self, enum_rep: str, string_rep: str):
        self.enum_rep = enum_rep
        self.string_rep = string_rep

    def __repr__(self):
        return f'{self.enum_rep}: {self.string_rep}'

def file_output(lines, out_filename, enum_name, namespace, underlying_type):
    out = open(out_filename, "w")
    out.write("#pragma once\n")
    out.write("#include <string>\n")
    out.write("#include <unordered_map>\n")
    out.write("#include <utility>\n\n")
    out.write(f'namespace {namespace} {{\n')
    out.write(f'enum class {enum_name} : {underlying_type} {{\n')

    categories: list[Category] = []
    entries: list[Entry] = []
    active_categories: dict[str, Category] = {}

    for raw_line in lines:
        strip_line = raw_line.strip()
        if len(strip_line) == 0:
            continue

        first, second = strip_line.split()
        if first == "!BEGIN":
            new_category = Category("", len(entries), 0)
            active_categories[second] = new_category
            categories.append(new_category)
        elif first == "!END":
            active_categories[second].end = len(entries)
            active_categories.pop(second)
        else:
            entries.append( Entry(first, second.removeprefix('!')) )


    print(categories)
    print(entries)
    out.close()
    pass

def file_generate(filename):
    file = open(filename)
    lines = list(file)
    file.close()

    if len(lines) == 0:
        print(f'Empty file: {filename}')
        exit(1)

    header = lines[0].strip()
    if len(header) == 0:
        print("First line must contain file name, enum name, namespace, and underlying type.")
        exit(1)

    fn, en, ns, un = header.split()
    file_output(lines[1:], fn, en, ns, un)
    pass

arg_len = len(sys.argv)
if arg_len == 1:
    print("Provide filename(s) as argument(s).")
    exit(1)

filenames = sys.argv[1:]

for filename in filenames:
    file_generate(filename)
