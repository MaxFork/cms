#include "translator.hpp"
#include "language.hpp"
#include "core/logger.hpp"

#include <stdio.h>
#ifdef PLATFORM_WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

TEGRA_USING_NAMESPACE Tegra::eLogger;

TEGRA_NAMESPACE_BEGIN(Tegra::Translation)

Translator::Translator()
{
    m_translatorStruct = new TranslatorStruct();
    jsonParser = std::make_unique<JSon>(); // jsonParser is a unique_ptr that owns a JSon
    m_defaultLanguage = basicLang;
}

Translator::~Translator()
{
    __tegra_safe_delete(m_translatorStruct);
}

//!TODO for better init by external devices.
bool Translator::initExternal(const std::vector<std::string>& file) __tegra_noexcept
{
    bool res = false;
    try {
        for(const auto& f : file) {
            jsonParser->push_back(JSon::parse(f.c_str()));
        }
        m_hasError = false;
        res = true;
    } catch (JSonException& e) {
        Log("Error Message " + std::string(e.what()), LoggerType::Critical);
        m_hasError = true;
        m_errorMessage = std::string(e.what());
        res = false;
    }
    return res;
}

bool Translator::init() __tegra_noexcept
{
    bool res = false;
    for(const auto& f : getFile()) {
        try {
            std::string file = { std::string(Multilangual::LanguagePath::getExecutablePath()) + translations + "/" + std::string(f) + ".json"};
            if(Multilangual::LanguagePath::exists(file)) {
                std::ifstream i(file);
                jsonParser->push_back(JSon::parse(i));
                m_hasError = false;
                res = true;
            } else {
                Log("The file [" + file + "] does not exist!", LoggerType::Critical);
            }
        } catch (JSonException& e) {
            Log("Error Message " + std::string(e.what()), LoggerType::Critical);
            m_hasError = true;
            m_errorMessage = std::string(e.what());
            res = false;
        }
    }
    return res;
}

bool Translator::existFile(const std::string& file) __tegra_const_noexcept
{
    return Multilangual::LanguagePath::exists(file) ? true : false;
}

bool Translator::isMultiLanguage() __tegra_const_noexcept
{
    bool ml = {false};
    if (m_multi_language) {
        ml = true;
    } else {
        ml = false;
    }
    return ml;
}

bool Translator::isValid() __tegra_noexcept
{
    bool ret = {false};
    std::string filePath{};
    std::ifstream valid;
    valid.exceptions(std::ifstream::badbit);
    try {
#if !defined(PLATFORM_DESKTOP) && defined(PLATFORM_ANDROID)
        char buff[FILENAME_MAX];
        //filePath = { std::string(GetCurrentDir(buff, FILENAME_MAX)) + translations + "/" + std::string(file)};
        std::string filePath = { "assets:/translations/" + std::string(file)};
#elif !defined(PLATFORM_DESKTOP) && defined(PLATFORM_IOS)
        //TODO...
        std::clog << "check real path for iOS from engine!\n";
        char buff[FILENAME_MAX];
        filePath = { std::string(GetCurrentDir(buff, FILENAME_MAX)) + translations + "/" + std::string(file)};
#elif defined(PLATFORM_DESKTOP)
        namespace fs = std::filesystem;
        filePath = { std::string(fs::current_path()) + translations + "/" + std::string(file)};
#else
#endif
        m_hasError = false;
        if (existFile(filePath)) {
            valid.open(filePath);
            if (!valid.is_open()) {
                m_hasError = true;
                m_errorMessage = "Could not open the file!\n";
                Log("Could not open the file!" , LoggerType::Critical);
            } else if (valid.bad()) {
                m_hasError = true;
                m_errorMessage = "Problem while reading file!\n";
                Log("Problem while reading file!" , LoggerType::Critical);
            }
        } else {
            m_hasError = true;
            m_errorMessage = "There is no file!\n";
            Log("There is no file!" , LoggerType::Critical);
        }
    } catch (const std::invalid_argument& e) {
        Log("Error " + std::string(e.what()), LoggerType::Critical);
        m_errorMessage = std::string(e.what());
        m_hasError = true;
    }
    ret = !jsonParser->accept(valid) ? false : true;
    return ret;
}

bool Translator::hasError() __tegra_const_noexcept
{
    return m_hasError;
}

std::string Translator::errorMessage() __tegra_const_noexcept
{
    return m_errorMessage;
}

bool Translator::hasString(const std::string& lang, const std::string& sheet, const std::string& key) __tegra_const_noexcept
{
    try {
        auto const l = wordMap.at(lang).at(sheet);
        return l.find(key) != std::cend(l);
    } catch (...) {
        return false;
    }
}

LanguageList Translator::list() noexcept
{
    auto items = *jsonParser;
    for(const auto& root : items) {
        for(const auto& l : root["language-spec"]["name"])
        {
            m_list.push_back(l.get<std::string>());
        }
    }
    return m_list;
}

bool Translator::isRtl(const std::string& code) __tegra_noexcept
{
    auto items = *jsonParser;
    for(const auto& root : items) {
        if(root["language-spec"]["code"] == code)
            for(const auto& l : root["language-spec"]["rtl"])
            {
                m_translatorStruct->isRtl = l.get<bool>();
            }
    }
    return m_translatorStruct->isRtl;
}

std::string Translator::symbol(const std::string& code) __tegra_noexcept
{
    auto items = *jsonParser;
    for(const auto& root : items) {
        if(root["language-spec"]["code"] == code)
            for(const auto& l : root["language-spec"]["code"])
            {
                m_translatorStruct->symbol = l.get<std::string>();
            }
    }
    return m_translatorStruct->symbol;
}

std::string Translator::currency(const std::string& code) __tegra_noexcept
{
    auto items = *jsonParser;
    for(const auto& root : items) {
        if(root["language-spec"]["code"] == code)
            for(const auto& l : root["language-spec"]["currency"])
            {
                m_translatorStruct->currency = l.get<std::string>();
            }
    }
    return m_translatorStruct->currency;
}

std::string Translator::callingCode(const std::string& code) __tegra_noexcept
{
    auto items = *jsonParser;
    for(const auto& root : items) {
        if(root["language-spec"]["code"] == code)
            for(const auto& l : root["language-spec"]["calling_code"])
            {
                m_translatorStruct->callingCode = l.get<std::string>();
            }
    }
    return m_translatorStruct->callingCode;
}

std::string Translator::callingCodeByUri(const std::string& code) __tegra_noexcept
{
    auto items = *jsonParser;
    for(const auto& root : items) {
        if(root["language-spec"]["uri"] == code)
            for(const auto& l : root["language-spec"]["code"]) {
                m_translatorStruct->callingCode = l.get<std::string>();
            }
    }
    return m_translatorStruct->callingCode;
}

std::string Translator::drivingSide(const std::string& code) __tegra_noexcept
{
    auto items = *jsonParser;
    for(const auto& root : items) {
        if(root["language-spec"]["code"] == code)
            for(const auto& l : root["language-spec"]["driving_side"])
            {
                m_translatorStruct->drivingSide = l.get<std::string>();
            }
    }
    return m_translatorStruct->drivingSide;
}

std::string Translator::iso3166Code(const std::string& code) __tegra_noexcept
{
    auto items = *jsonParser;
    for(const auto& root : items) {
        if(root["language-spec"]["code"] == code)
            for(const auto& l : root["language-spec"]["iso_3166_code"])
            {
                m_translatorStruct->iso3166Code = l.get<std::string>();
            }
    }
    return m_translatorStruct->iso3166Code;
}


std::string Translator::internetTld(const std::string& code) __tegra_noexcept
{
    auto items = *jsonParser;
    for(const auto& root : items) {
        if(root["language-spec"]["code"] == code)
            for(const auto& l : root["language-spec"]["internet_tld"])
            {
                m_translatorStruct->internetTld = l.get<std::string>();
            }
    }
    return m_translatorStruct->internetTld;
}

LanguageList Translator::listByCode() noexcept
{
    auto items = *jsonParser;
    for(const auto& root : items) {
        for(const auto& l : root["language-spec"]["code"])
        {
            m_translatorStruct->language.push_back(l.get<std::string>());
        }
    }
    return m_translatorStruct->language;
}

LanguageList Translator::listByTitle() noexcept
{
    auto items = *jsonParser;
    for(const auto& root : items) {
        for(const auto& l : root["language-spec"]["native_name"])
        {
            m_list.push_back(l.get<std::string>());
        }
    }
    return m_list;
}

LanguageFile Translator::getFile() __tegra_const_noexcept
{
    return m_translatorStruct->file;
}

void Translator::setFile(const LanguageFile &file) __tegra_noexcept
{
    if(!file.empty()) {
        m_translatorStruct->file = file;
    }
}

void Translator::wordProcess() __tegra_noexcept
{
    try {
        auto items = jsonParser->items();
        for (const auto& it : items) {
            LanguageSheet langSheet;
            for (const auto& [key, value] : it.value()["data"].items())
            {
                //! key values are: exceptions, global, languages, ...
                LanguageTemp temp;
                for (auto i : value) {
                    LanguageStruct ls;
                    ls.m_wordKey = i["word_key"].get<std::string>(),
                        ls.m_module = i["module"].get<std::string>(),
                        ls.m_defaultValue = i["default_value"].get<std::string>(),
                        ls.m_customValue = i["custom_value"].get<std::string>();
                    LanguageTemplate words(ls) ;
                    temp.insert(LanguagePair(i["word_key"].get<std::string>(), words));
                }
                langSheet[key] = temp;
            }
            wordMap[it.value()["language-spec"]["code"]] = langSheet;
        }
        m_hasError = false;

    } catch (const std::range_error& e) {
        Log("Error " + std::string(e.what()), LoggerType::Critical);
        m_errorMessage = std::string(e.what());
        m_hasError = true;
    }
}

bool Translator::parse() noexcept
{
    bool res = {false};
    if (init()) {
        wordProcess();
        res = true;
        m_translatorStruct->parseMessage = "Parsing initialized!";
        Log("Parsing initialized!", LoggerType::Success);
    } else {
        res = false;
        m_translatorStruct->parseMessage = "Parse error!";
        Log("Parse error!", LoggerType::Critical);
    }
    return res;
}

bool Translator::parseExternal(const std::vector<std::string> &data) __tegra_noexcept
{
    bool res = {false};
    if (initExternal(data)) {
        wordProcess();
        res = true;
        m_translatorStruct->parseMessage = "Parsing initialized!";
        Log("Parsing initialized!", LoggerType::Success);
    } else {
        res = false;
        m_translatorStruct->parseMessage = "Parse error!";
        Log("Parse error!", LoggerType::Critical);
    }
    return res;
}

void Translator::parseMessage() noexcept
{
    Log(m_translatorStruct->parseMessage, LoggerType::Info);
}

std::string Translator::defaultLanguage() __tegra_const_noexcept
{
    if (m_defaultLanguage.empty()) {
        return basicLang.data();
    } else {
        return m_defaultLanguage;
    }
}

LanguageTemplate Translator::translate(const std::string& lang, const std::string& sheet, const std::string& key)
{
    if (!isMultiLanguage()) {
        try {
            return wordMap.at(defaultLanguage()).at(sheet).at(key);
            m_hasError = false;
        } catch (const std::out_of_range& e) {
            Log("Error Message: " + std::string(e.what()), LoggerType::Warning);
            m_errorMessage = std::string(e.what());
            m_hasError = true;
            return m_template;
        }
    } else {
        try {
            return wordMap.at(lang).at(sheet).at(key);
            m_hasError = false;
        } catch (const std::out_of_range& e) {
            Log("Error Message: " + std::string(e.what()), LoggerType::Warning);
            m_errorMessage = std::string(e.what());
            m_hasError = true;
            return m_template;
        }
    }
}

DictonaryType Translator::data(const std::string& sheet) __tegra_noexcept
{
    DictonaryType d;
    auto items = *jsonParser;
    for(const auto& root : items) {
        for(const auto& i : root["data"][sheet]) {
            if (i.is_object()) {
                d.insert(
                    std::pair(root["language-spec"]["code"],
                        std::pair(i["word_key"].get<std::string>(),
                        i["default_value"].get<std::string>()
                    )));
            }
        }
    }
    return d;
}

TEGRA_NAMESPACE_END
