#ifndef __XMLWRITER_H__
#define __XMLWRITER_H__

#include <string>
#include <vector>

namespace Connectivity
{

    class AvXmlDocument;

    class AvXmlElementRef
    {
      private:
        int m_iIndex;
        AvXmlDocument *m_rAvXmlDocument;

        friend class AvXmlDocument;
        friend class AvXmlElement;

      public:
        AvXmlElementRef(size_t index, AvXmlDocument *doc) : m_iIndex(index), m_rAvXmlDocument(doc)
        {
        }
        void setAttribute(const std::string &key, const std::string &value);
        void setAttribute(const std::string &key, size_t value);
        void setText(std::string txt);
        void setSelfClose(bool close = true);
        AvXmlElementRef insertChildElement(std::string name);
    };

    class AvXmlElement
    {
        std::string m_sName;
        std::vector<std::pair<std::string, std::string>> m_vecAttributes;
        std::string m_sText;
        std::vector<AvXmlElementRef> m_vecChildren;
        bool m_bIsSelfClosing;

        friend class AvXmlElementRef;
        friend class AvXmlDocument;

      public:
        AvXmlElement(const std::string &name);
        std::string toString() const;
    };

    class AvXmlDocument
    {
        std::vector<AvXmlElement> m_vecNodePool;
        friend class AvXmlElementRef;

        void printElement(std::ofstream &out, const AvXmlElementRef &curr, int deep) const;

      public:
        AvXmlDocument(const std::string &rootName);
        int saveFile(const std::string &fileName) const;
        std::string toString() const;
        AvXmlElementRef getRootElement() const;
    };

} // namespace Connectivity

#endif