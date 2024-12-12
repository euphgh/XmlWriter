#ifndef __AVXMLDOCUMENT__
#define __AVXMLDOCUMENT__

#include <memory>
#include <string>
#include <vector>

namespace Connectivity
{

    class AvXmlDocument;
    class AvXmlElement;

    using AvXmlElementRef = std::shared_ptr<AvXmlElement>;
    class AvXmlElement
    {
        std::string m_sName;
        std::vector<std::pair<std::string, std::string>> m_vecAttributes;
        std::string m_sText;
        std::vector<std::shared_ptr<AvXmlElement>> m_vecChildren;
        bool m_bIsSelfClosing;

        friend class AvXmlDocument;

      public:
        AvXmlElement(const std::string &name);
        std::string toString() const;
        void setAttribute(const std::string &key, const std::string &value);
        void setAttribute(const std::string &key, size_t value);
        void setText(std::string txt);
        void setSelfClose(bool close = true);
        AvXmlElementRef insertChildElement(std::string name);
    };

    class AvXmlDocument
    {
        AvXmlElementRef m_pRootElement;

        void printElement(std::ofstream &out, const AvXmlElementRef &curr, int deep) const;

      public:
        AvXmlDocument(const std::string &rootName);
        int saveFile(const std::string &fileName) const;
        AvXmlElementRef getRootElement() const;
    };

} // namespace Connectivity

#endif