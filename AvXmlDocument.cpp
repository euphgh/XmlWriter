#include "AvXmlDocument.h"
#include <fstream>
#include <sstream>

namespace Connectivity
{

    using std::ofstream;
    using std::string;

    AvXmlElement::AvXmlElement(const std::string &name) : m_sName(name), m_bIsSelfClosing(true)
    {
    }

    void AvXmlElementRef::setAttribute(const string &key, const string &value)
    {
        m_rAvXmlDocument->m_vecNodePool[m_iIndex].m_vecAttributes.emplace_back(key, value);
    }

    void AvXmlElementRef::setText(string text)
    {
        m_rAvXmlDocument->m_vecNodePool[m_iIndex] = text;
        if (text != "")
            m_rAvXmlDocument->m_vecNodePool[m_iIndex].m_bIsSelfClosing = false;
    }

    void AvXmlElementRef::setSelfClose(bool close)
    {
        m_rAvXmlDocument->m_vecNodePool[m_iIndex].m_bIsSelfClosing = close;
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
            childRes << m_vecChildren[i].m_iIndex << ", ";
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

    AvXmlElementRef AvXmlElementRef::insertChildElement(string name)
    {
        m_rAvXmlDocument->m_vecNodePool.emplace_back(name);
        m_rAvXmlDocument->m_vecNodePool[m_iIndex].m_bIsSelfClosing = false;
        m_rAvXmlDocument->m_vecNodePool[m_iIndex].m_vecChildren.emplace_back(m_rAvXmlDocument->m_vecNodePool.size() - 1, m_rAvXmlDocument);
        return AvXmlElementRef(m_rAvXmlDocument->m_vecNodePool.size() - 1, m_rAvXmlDocument);
    }

    void AvXmlDocument::printElement(ofstream &out, const AvXmlElementRef &ref, int deep) const
    {
        const auto curr = m_vecNodePool[ref.m_iIndex];
        out << string(deep * 2, ' ') << '<' << curr.m_sName;
        for (const auto &[key, value] : curr.m_vecAttributes)
        {
            out << ' ' << key << "=\"" << value << "\"";
        }
        if (curr.m_bIsSelfClosing)
        { // no text not child
            out << "/>\n";
        }
        else
        {
            out << '>' << curr.m_sText << '\n';
            for (const auto child : curr.m_vecChildren)
            {
                printElement(out, child, deep + 1);
            }
            out << "</" << curr.m_sName << ">\n";
        }
    }

    AvXmlDocument::AvXmlDocument(const string &rootName) : m_vecNodePool({rootName})
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

    string AvXmlDocument::toString() const
    {
        std::stringstream res("");
        for (size_t i = 0; i < m_vecNodePool.size(); i++)
        {
            res << "node[" << i << "]: " << m_vecNodePool[i].toString() << "%s\n";
        }
        return res.str();
    }

    AvXmlElementRef AvXmlDocument::getRootElement() const
    {
        return AvXmlElementRef(0, (AvXmlDocument *)this);
    }
} // namespace Connectivity
