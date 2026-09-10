#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_set>

#include "stdlib.h"
#include "json.hpp"
#include "novotts_api.h"

using namespace std;
using json = nlohmann::json;






std::vector<std::string> SplitUtf8(const std::string& s) {
    std::vector<std::string> result;
    std::string current;

    static const std::unordered_set<std::string> delimiters = {
        "，", "。", ",", ".", "?", ";", ":", "：", "？", "；"
    };

    size_t i = 0;
    while (i < s.size()) {
        size_t char_len = 1;
        unsigned char c = s[i];
        if      ((c & 0x80) == 0)       char_len = 1;        // 0xxxxxxx
        else if ((c & 0xE0) == 0xC0)    char_len = 2;        // 110xxxxx
        else if ((c & 0xF0) == 0xE0)    char_len = 3;        // 1110xxxx
        else if ((c & 0xF8) == 0xF0)    char_len = 4;        // 11110xxx

        std::string ch = s.substr(i, char_len);

        if (delimiters.count(ch)) {
            if (!current.empty()) {      // ← 如需保留空段，删掉这行 if
                result.push_back(current);
                current.clear();
            }
        } else {
            current += ch;
        }
        i += char_len;
    }

    if (!current.empty()) {
        result.push_back(current);
    }
    return result;
}



int main(int argc, char *argv[])
{
  void *handler = NULL;

  if (argc < 2)
  {
    std::cerr << "Need config path" << std::endl;
    return 1;
  }

  if (argc < 3)
  {
    std::cerr << "Need speaker path" << std::endl;
    return 1;
  }

  if (argc < 4)
  {
    std::cerr << "Need output WAV path" << std::endl;
    return 1;
  }

  auto configpath = std::string(argv[1]);
  auto speaker = std::string(argv[2]);
  auto outputPath = std::string(argv[3]);

  novottsOnnxInit(configpath.c_str());
  novottsEngineInit(&handler, configpath.c_str(), speaker.c_str());


  
  int audiosize = 0;
  std::string text = "Today is Thursday. It is partly cloudy in ShenZhen today, with the temperature ranging from 26℃ to 30℃. The east wind blows at 4‑6 levels, and the air is quite humid. There is a chance of showers in the afternoon. It feels hot and sticky. Remember to bring an umbrella if you go out.";
  vector<string> textArray = SplitUtf8(text);

  FILE * fp = fopen(outputPath.c_str(), "wb");
  std::cout << "tts text start" << std::endl;
  for(auto item: textArray){
    unsigned short *audio = nullptr;
    novottsSynthesize(handler, item.c_str(), &audio, &audiosize);
    fwrite(audio, audiosize, 2, fp);
    novottsReturn(audio);
    std::cout << "tts text:" << item << std::endl;
  }
  
  fclose(fp);
  

  novottsEngineUninit(handler);
  novottsOnnxRelease();
  std::cout << "OK" << std::endl;

  return EXIT_SUCCESS;
}
