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

#ifndef TEMPLATES_HPP
#define TEMPLATES_HPP

#include "common.hpp"
#include "core/core.hpp"
#include "seo.hpp"

TEGRA_NAMESPACE_BEGIN(Tegra::CMS)

#define CMS_THIRD_PARTY_JAVASCRIPT  "thirdPartyJs"
#define CMS_THIRD_PARTY_CSS         "thirdPartyCss"
#define CMS_SYSTEM_SHEET            "systemStyle"
#define CMS_LINK_SHEET              "linkSheet"
#define CMS_META_SHEET              "meta"

using StaticMap = std::map<std::string, std::string>;

enum class UserType : u8
{
    Guest,
    User,
    Admin,
    Moder,
    Extra
};

enum class FileType : u8
{
    HTML,
    XHTML,
    CSS,
    JavaScript
};

enum class UIFramework : u8
{
    Self,
    ThirdParty
};

enum class CDN : u8
{
    Internal,
    External
};

/*!
 * @brief The Template class
 */
class Template
{
public:

    /*!
     * \brief Template class represents the template style and content decoration mode.
     * \param usertype is for template model such as Admin, User and etc.
     * \param p is path as string that is taken from the input.
     */
    Template();
    ~Template();
    /*!
     * \brief The Requirments struct
     */
    struct Requirments
    {
        Tegra::Types::ConfigType config;
        Tegra::SEO::StaticMeta staticMeta;
    };

    /** Template source folder */
    const static inline std::string Source    = "templates";

    /** Static assets folder folder */
    const static inline std::string Assets    = "assets";

    /** User template folder */
    const static inline std::string User      = "User";

    /** Admin template folder */
    const static inline std::string Admin     = "Admin";

    /** Extra panel template folder */
    const static inline std::string Moder     = "Moderator";

    /** Type of files to be processed */
    inline const static std::string fileType  = {".html"};
    inline const static std::string cssRoot   = {".css"};
    inline const static std::string cspRoot   = {".csp"};
    inline const static std::string jsRoot    = {".js"};
    inline const static std::string fontRoot  = {".font"};

    std::string theme() const;

    std::string font() const;

    std::string css() const;

    std::string script() const;

    std::string gg() const;

    std::vector<std::string> styleSheet;

    std::vector<std::string> javaScript;

    std::vector<std::string> linkSheet;

    std::vector<std::string> systemSheet;

    bool fileExist(const std::string& file);

    Tegra::SEO::StaticMeta staticMeta;

private:
    UserType utype;
    Scope<Engine> engine;
    Framework::HttpRequestPtr req;
};

class LoadListTemplate;
/*!
 * @brief The LoadListTemplate class
 * @details this class provides template meta tag and links inside of HTML
 * document.
 */
class LoadListTemplate
{
public:

    struct BasicStruct final
    {
        std::string title{};
        std::string description{};
        std::string seprator{};
        std::string fullSiteTitle{};
    };

    LoadListTemplate(const std::string &l, const std::string &p);
    ~LoadListTemplate();

    /*!
     * \brief return title of HTML document.
     */
    std::optional<std::string> title() const;

    /*!
     * \brief return description of HTML document.
     */
    std::optional<std::string> description() const;

    /*!
     * \brief return seprator character between title and description.
     */
    std::optional<std::string> siteSeprator() const;

    /*!
     * \brief return full website title.
     */
    std::optional<std::string> fullSiteTitle() const;

protected:
    /*!
     * \brief Sets a title for HTML document.
     * \return string.
     */
    __tegra_maybe_unused void setTitle(const std::string& val);

    /*!
     * \brief Sets a description for HTML document.
     * \return string.
     */
    __tegra_maybe_unused void setDescription(const std::string& val);


    /*!
     * \brief Sets a seprator character between title and descriptio for HTML document.
     * \return string.
     */
    __tegra_maybe_unused void setSiteSeprator(const std::string& val);

    /*!
     * \brief Sets full string of title and description.
     * \return string.
     */
    __tegra_maybe_unused void setFullSiteTitle(const std::string& val);

public:
    static inline std::map<std::string, std::string> metaData;

private:
    std::string   m_title{};
    std::string   m_description{};
    std::string   m_siteSeprator{};
    std::string   m_fullSiteTitle{};
};

TEGRA_NAMESPACE_END

#endif  // TEMPLATES_HPP
