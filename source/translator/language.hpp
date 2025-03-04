/*!
 * MIT License
 *
 * Copyright (c) 2022 Kambiz Asadzadeh
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LANGUAGE_HPP
#define LANGUAGE_HPP

#include "common.hpp"

TEGRA_NAMESPACE_BEGIN(Tegra::Multilangual)

class LanguagePath
{
public:
    LanguagePath() = default;
    ~LanguagePath() = default;

    /*!
     * \brief getExecutablePath
     * \return current executable file path.
     */
    __tegra_no_discard static std::string getExecutablePath();

    /*!
     * \brief exists
     * \return true if file is exist in the path.
     */
    __tegra_no_discard static bool exists(const std::string& file);

};

/*! Declaration of language support */

class Language {
public:
    Language();
    ~Language();

    /*!
     * \brief set function will sets all data from translator adaptor inside the system.
     * \param data as a map [list].
     */
    void registerAll(const Types::LanguageType& data);

    /*!
     * \brief registerLanguage function will sets new language to language datasheets.
     * \param code is language standard code.
     */
    void registerLanguage(const Types::CodeType& code);

    /*!
     * \brief registerSections
     * \param sec
     */
    void registerSections(const Types::SectionType& sec);

    /*!
     * \brief languageSupport
     * \returns
     */
    Types::CodeType languageSupport();

    /*!
     * \brief sections
     * \returns
     */
    Types::SectionType sections();

    /*!
     * \brief getLanguageCode
     * \returns
     */
    static std::string getLanguageCode();

    /*!
     * \brief getLanguage
     * \returns
     */
    static std::string getLanguage();

    /*!
     * \brief get function will returns all data.
     * \returns as map.
     */
    __tegra_no_discard Types::LanguageType get() __tegra_const_noexcept;

private:
    Types::LanguageType       m_get;
    Types::CodeType           m_languageSupport;
    Types::SectionType        m_sections;
};

TEGRA_NAMESPACE_END

#endif  // LANGUAGE_HPP
