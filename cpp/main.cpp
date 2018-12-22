//
//  main.cpp
//  TestRegex
//
//  Created by Владимир Толмачев on 20/12/2018.
//  Copyright © 2018 Владимир Толмачев. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <regex>
#include <algorithm>
#include <numeric>

auto regexs = {
    std::make_pair(std::regex(R"(DataStorage::shared\(\).get<(\w+)>)"), "DataStorage::shared().get$1"),
    std::make_pair(std::regex(R"(Factory::(.+)<\w+>)"), "Factory.$1"),
    std::make_pair(std::regex(R"(for\s*\(\s*\w+[\s&\*]*(\w+)\s*:\s*(.+)\s*\))"), "for $1 in $2:"),
    std::make_pair(std::regex(R"(for\s*\(\s*\w+\s*(\w+)=(\w+);\s*\w+<(\w+);\s*\+\+\w+\s*\))"), "for $1 in range($2, int($3)):"),
    std::make_pair(std::regex(R"(for\s*\(\s*\w+\s*(\w+)=(\w+);\s*\w+>(\w+);\s*--\w+\s*\))"), "for $1 in range($2, int($3), -1):"),
    std::make_pair(std::regex(R"(for\s*\(\s*\w+\s*(\w+)=(\w+);\s*\w+<(\w+);\s*\w+\+=(\w)\s*\))"), "for $1 in range($2, int($3), $4):"),
    std::make_pair(std::regex(R"(for\s*\(\s*\w+\s*(\w+)=(\w+);\s*\w+>(\w+);\s*\w+-=(\w)\s*\))"), "for $1 in range($2, int($3), -$4):"),
    std::make_pair(std::regex(R"(\bwhile\s*\((.+)\))"), "while($1):"),
    std::make_pair(std::regex(R"(else\s+if\s*\(\s*(.+)\s*\))"), "elif $1:"),
    std::make_pair(std::regex(R"(if\s*\(\s*(.+)\s*\))"), "if $1:"),
    std::make_pair(std::regex(R"(if\s*!(.+):)"), "if not $1:"),
    std::make_pair(std::regex(R"(else)"), "else:"),
    std::make_pair(std::regex(R"(in_map\s*\(\s*(.+),\s*(.+)\s*\))"), "($1 in $2)"),
    std::make_pair(std::regex(R"(in_list\s*\(\s*(.+),\s*(.+)\s*\))"), "($1 in $2)"),
    std::make_pair(std::regex(R"(list_push\s*\(\s*(.+),\s*(.+)\s*\))"), "$1.append($2)"),
    std::make_pair(std::regex(R"(list_remove\s*\(\s*(.+),\s*(.+)\s*\))"), "$1.remove($2)"),
    std::make_pair(std::regex(R"(list_clear\s*\(\s*(.+)\s*\))"), "$1 = list()"),
    std::make_pair(std::regex(R"(list_size\s*\()"), "len("),
    std::make_pair(std::regex(R"(map_size\s*\()"), "len("),
    std::make_pair(std::regex(R"(string_empty\((.+?)\))"), "(not ($1))"),
    std::make_pair(std::regex(R"(string_size\((.+?)\))"), "len($1)"),
    std::make_pair(std::regex(R"((\w+)\s+(\w+);)"), "$2 = $1()"),
    std::make_pair(std::regex(R"((\w+) = return\(\))"), "return $1"),
    std::make_pair(std::regex(R"(std::vector<.+>\s+(\w+))"), "$1 = list()"),
    std::make_pair(std::regex(R"(std::string\s+(\w+))"), "$1"),
    std::make_pair(std::regex(R"(\blist<.+>\s+(\w+))"), "$1 = list()"),
    std::make_pair(std::regex(R"(\bmap<([<:>\w\s\*&]+),\s*([<:>\w\s\*&]+)>\s*(\w+))"), "$3 = dict()"),
    std::make_pair(std::regex(R"(auto\&* (\w+))"), "$1"),
    std::make_pair(std::regex(R"(string (\w+))"), "$1"),
    std::make_pair(std::regex(R"(int (\w+))"), "$1"),
    std::make_pair(std::regex(R"(float (\w+))"), "$1"),
    std::make_pair(std::regex(R"(bool (\w+))"), "$1"),
    std::make_pair(std::regex(R"((\w)->)"), "$1."),
    std::make_pair(std::regex(R"(\+\+(\w+))"), "$1 += 1"),
    std::make_pair(std::regex(R"((\w+)\+\+)"), "$1 += 1"),
    std::make_pair(std::regex(R"(delete (\w*);)"), "pass"),
    std::make_pair(std::regex(R"(&(\w+))"), "$1"),
    std::make_pair(std::regex(R"(!(\w+))"), "not $1"),
    std::make_pair(std::regex(R"(!\()"), "not ("),
    std::make_pair(std::regex(R"(make_intrusive<(\w+)>\(\))"), "$1()"),
    std::make_pair(std::regex(R"(new\s*(\w+)\s*\(\s*\))"), "$1()"),
    std::make_pair(std::regex(R"(assert\(.+\);)"), ""),
    std::make_pair(std::regex(R"((\b[-0-9]+)\.f\b)"), "$1.0"),
    std::make_pair(std::regex(R"((\b[-0-9]+)\.([-0-9]*)f\b)"), "$1.$2"),
    std::make_pair(std::regex(R"(;)"), ""),
    std::make_pair(std::regex(R"(([*+-/\s])log\((.+?)\))"), "$1math.log($2)"),
    std::make_pair(std::regex(R"(random_float\(\))"), "random.random()"),
    std::make_pair(std::regex(R"(random_int\((.+)?,\s*(.+)?\))"), "random.randint($1, $2 - 1)"),
    std::make_pair(std::regex(R"(\bthis\b)"), "self"),
    std::make_pair(std::regex(R"(, std::placeholders::_\d)"), ""),
    std::make_pair(std::regex(R"(dynamic_pointer_cast_intrusive<\w+>\((.+?)\))"), "$1"),
    std::make_pair(std::regex(R"(([\w\.]+?)\s*!=\s*False)"), "($1)"),
    std::make_pair(std::regex(R"(([\w\.]+?)\s*==\s*False)"), "not ($1)"),
    std::make_pair(std::regex(R"(\bstrTo<(\w+)>\((.+?)\))"), "$1($2)"),
    std::make_pair(std::regex(R"(\btoStr\((.+?)\))"), "str($1)"),
    std::make_pair(std::regex(R"(std::strcat\((.+?),\s*(.+?)\))"), "(($1)+($2))"),
    std::make_pair(std::regex(R"(\.at\((.*?)\))"), "[$1]"),
    
    std::make_pair(std::regex(R"(DataStorage::shared\(\).get<(\w+)>)"), "DataStorage::shared()->get$1"),
    std::make_pair(std::regex(R"(Factory::(.+)<\w+>)"), "Factory::$1"),
    std::make_pair(std::regex(R"(\.str\(\))"), ""),
    std::make_pair(std::regex(R"(for\s*\(auto (.+?)\s*:\s*(.+)\s*\))"), "foreach($$2 as $$1)"),
    std::make_pair(std::regex(R"(for\s*\(auto& (.+?)\s*:\s*(.+)\s*\))"), "foreach($$2 as $$1)"),
    std::make_pair(std::regex(R"(for\s*\(auto&&\s*\[(\w+),\s*(\w+)\]\s*:\s*(.+)\))"), "foreach ($$3 as $$1 => $$2)"),
    std::make_pair(std::regex(R"(auto (\w+))"), "$$1"),
    std::make_pair(std::regex(R"(auto& (\w+))"), "$$1"),
    std::make_pair(std::regex(R"(void (\w+))"), "$$1"),
    std::make_pair(std::regex(R"(int (\w+))"), "$$1"),
    std::make_pair(std::regex(R"(bool (\w+))"), "$$1"),
    std::make_pair(std::regex(R"(\((\w+) (\w+)\))"), "($$2)"),
    std::make_pair(std::regex(R"(\(const (\w+)\& (\w+)\))"), "($$2)"),
    std::make_pair(std::regex(R"(\(const (\w+)\* (\w+)\))"), "($$2)"),
    std::make_pair(std::regex(R"(\((\w+)\* (\w+)\))"), "($$2)"),
    std::make_pair(std::regex(R"((\w+)\ (\w+),)"), "$$2,"),
    std::make_pair(std::regex(R"((\w+)\& (\w+),)"), "$$2,"),
    std::make_pair(std::regex(R"((\w+)\* (\w+),)"), "$$2,"),
    std::make_pair(std::regex(R"(const (\w+)\* (\w+))"), "$$2"),
    std::make_pair(std::regex(R"(const (\w+)\& (\w+))"), "$$2"),
    std::make_pair(std::regex(R"(float (\w+))"), "$$1"),
    std::make_pair(std::regex(R"(std::string (\w+))"), "$$1"),
    std::make_pair(std::regex(R"(\bthis\b)"), "$this"),
    std::make_pair(std::regex(R"(:const)"), ""),
    std::make_pair(std::regex(R"((\w+)::(\w+))"), "$1::$$2"),
    std::make_pair(std::regex(R"((\w+)::(\w+)\))"), "$1::$$2)"),
    std::make_pair(std::regex(R"((\w+)::(\w+)\.)"), "$1::$$2."),
    std::make_pair(std::regex(R"((\w+)::(\w+)->)"), "$1::$$2->"),
    std::make_pair(std::regex(R"((\w+)::(\w+)\])"), "$1::$$2]"),
    std::make_pair(std::regex(R"((\w+)::\$(\w+)\()"), "$1::$2("),
    std::make_pair(std::regex(R"((\w+)::\$(\w+)\((\w*)\))"), "$1::$2($3)"),
    std::make_pair(std::regex(R"(function \$(\w+))"), "function $1"),
    std::make_pair(std::regex(R"(\.at\((.*?)\))"), "[$1]"),
    std::make_pair(std::regex(R"((\w+)\.)"), "$1->"),
    std::make_pair(std::regex(R"((\w+)\(\)\.)"), "$1()->"),
    std::make_pair(std::regex(R"((\w+)\]\.)"), "$1]->"),
    std::make_pair(std::regex(R"(&(\w+))"), "$1"),
    std::make_pair(std::regex(R"(\$if\()"), "if("),
    std::make_pair(std::regex(R"(delete \$(\w+);)"), ""),
    std::make_pair(std::regex(R"(([-0-9])->([-0-9])f\b)"), "$1.$2"),
    std::make_pair(std::regex(R"(assert\(.+\);)"), ""),
    std::make_pair(std::regex(R"(make_intrusive<(\w+)>\(\s*\))"), "new $1()"),
    std::make_pair(std::regex(R"(dynamic_pointer_cast_intrusive<\w+>\((.+?)\))"), "$1"),
    std::make_pair(std::regex(R"(new\s*(\w+)\s*\(\s*\))"), "new $1()"),
    std::make_pair(std::regex(R"((.+?)\->push_back\((.+)\);)"), "array_push($1, $2);"),
    std::make_pair(std::regex(R"((\w+)\s+(\w+);)"), "$$2 = new $1();"),
    std::make_pair(std::regex(R"(\$(\w+) = new return\(\);)"), "return $1;"),
    std::make_pair(std::regex(R"(std::\$vector<.+?>\s+(\w+))"), "$$1 = array()"),
    std::make_pair(std::regex(R"(\blist<.+>\s+(\w+))"), "$$1 = array()"),
    std::make_pair(std::regex(R"(\bmap<([<:>\w\s\*&\$]+),\s*([<:>\w\s\*&\$]+)>\s*(\w+))"), "$$3 = array()"),
    std::make_pair(std::regex(R"(\bstrTo<(\w+)>)"), "($1)"),
    std::make_pair(std::regex(R"(\btoStr\b)"), "(string)"),
    std::make_pair(std::regex(R"((@__string_\d+__)\s*\+)"), "$1."),
    std::make_pair(std::regex(R"(\+\s*(@__string_\d+__))"), ".$1"),
    std::make_pair(std::regex(R"(->\$(\w+)\()"), "->$1("),
    std::make_pair(std::regex(R"(([-0-9]*)->([-0-9]*)f\b)"), "$1.$2"),
    std::make_pair(std::regex(R"(([-0-9]*)->f\\b)"), "$1.0"),
    std::make_pair(std::regex(R"(\$return\s)"), "return"),
    std::make_pair(std::regex(R"((\$.+)->add\((\$.+),\s*(\w+)::\$(\w+),\s*std::\$placeholders::\$_\d\);)"), "$1->add($2, array($2, \"$4\"));"),
    std::make_pair(std::regex(R"(list_remove\((\$.+?),\s*(\$.+?)\);)"), "unset($1[array_search($2, $1)]);"),
    std::make_pair(std::regex(R"(list_clear\((.+?)\);)"), "$1 = array();"),
    std::make_pair(std::regex(R"(string_empty\((.+?)\))"), "(count($1) == 0)"),
    std::make_pair(std::regex(R"(random_float\(\))"), "(mt_rand() * 1.0 / mt_getrandmax())"),
    std::make_pair(std::regex(R"(random_int\((.+?),\s*(.+)\))"), "mt_rand($1, $2-1)"),
    std::make_pair(std::regex(R"(std::strcat\((.+?),\s*(.+?)\))"), "(($1).($2))"),
};

float iteration(){
    std::fstream stream("../../test_source.cpp", std::ios::in);
    if (stream.is_open() == false)
        return 1;
    std::string data((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    
    auto __start__ = std::chrono::system_clock::now();
    for(auto& pair : regexs)
    {
        data = std::regex_replace(data, pair.first, pair.second, std::regex_constants::match_any);
    }
    
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now()-__start__).count();
    return static_cast<float>(elapsed) / 1000000;
}

int main(int argc, const char * argv[]) {
    
    std::cout << "1 iteration: " << iteration() << "s" << std::endl;
    float avr = 0.f;
    for(int i=0; i<10; ++i){
        avr += iteration();
    }
    avr /= 10.f;
    std::cout << "10 iteration: " << avr << "s" << std::endl;
    
    return 0;
}
