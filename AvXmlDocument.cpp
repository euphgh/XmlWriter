#include "AvXmlDocument.h"
#include <cstddef>
#include <fstream>
#include <memory>
#include <sstream>

namespace Connectivity
{

    using std::ofstream;
    using std::string;

    AvXmlElement::AvXmlElement(const std::string &name) : m_sName(name), m_bIsSelfClosing(true)
    {
    }

    void AvXmlElement::setAttribute(const string &key, const string &value)
    {
        m_vecAttributes.emplace_back(key, value);
    }

    void AvXmlElement::setAttribute(const string &key, const size_t value)
    {
        setAttribute(key, std::to_string(value));
    }

    void AvXmlElement::setText(string text)
    {
        m_sText = text;
        if (text != "")
            m_bIsSelfClosing = false;
    }

    void AvXmlElement::setSelfClose(bool close)
    {
        m_bIsSelfClosing = close;
    }

    string AvXmlElement::toString() const
    {
        std::stringstream res("");
        res << "{ name = " << m_sName;
        res << ", selfClose = " << m_bIsSelfClosing;

        std::stringstream childRes("");
        childRes << "{ ";
        for (size_t i = 0; i < m_vecChildren.size(); i++)
        {
            childRes << m_vecChildren[i]->m_sName << ", ";
        }
        childRes << "}";

        res << ", children = " << childRes.str().c_str();

        std::stringstream attrRes("");
        attrRes << "{ ";
        for (size_t i = 0; i < m_vecAttributes.size(); i++)
        {
            attrRes << m_vecAttributes[i].first << " = " << m_vecAttributes[i].second << ", ";
        }
        attrRes << "}";
        res << ", attr = " << attrRes.str().c_str() << " }";

        return res.str().c_str();
    }

    AvXmlElementRef AvXmlElement::insertChildElement(string name)
    {
        m_bIsSelfClosing = false;
        const auto child = mkSharedPtr(name);
        m_vecChildren.emplace_back(child);
        return child;
    }

    void AvXmlDocument::printElement(ofstream &out, const AvXmlElementRef &ref, int deep) const
    {
        const auto curr = ref;
        out << string(deep * 2, ' ') << '<' << curr->m_sName;
        for (const auto &[key, value] : curr->m_vecAttributes)
        {
            out << ' ' << key << "=\"" << value << "\"";
        }
        if (curr->m_bIsSelfClosing)
        { // no text not child
            out << "/>\n";
        }
        else
        {
            out << '>' << curr->m_sText << '\n';
            for (const auto child : curr->m_vecChildren)
            {
                printElement(out, child, deep + 1);
            }
            out << string(deep * 2, ' ') << '<' << curr->m_sName << ">\n";
        }
    }

    AvXmlDocument::AvXmlDocument(const string &rootName) : m_pRootElement(mkSharedPtr(rootName))
    {
    }

    int AvXmlDocument::saveFile(const string &fileName) const
    {
        ofstream out(fileName);
        if (!out)
        {
            return 1;
        }
        printElement(out, getRootElement(), 0);
        return 0;
    }

    AvXmlElementRef AvXmlDocument::getRootElement() const
    {
        return m_pRootElement;
    }
} // namespace Connectivity
