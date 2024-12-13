#ifndef __AVXMLDOCUMENT__
#define __AVXMLDOCUMENT__

#include <cstddef>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace Connectivity
{
    template <typename T> class SharedPtrManager
    {
        std::vector<std::shared_ptr<T>> pools;

      public:
        template <typename... Args> std::shared_ptr<T> allocate(Args &&...args);
        void collect(std::shared_ptr<T> ptr);
    };

    template <typename T> class Parent
    {
      protected:
        std::shared_ptr<SharedPtrManager<T>> sharedPtrManager;

      public:
        template <typename... Args> std::shared_ptr<T> mkSharedPtr(Args &&...args);
        void collect(std::shared_ptr<T> ptr);
        Parent() : sharedPtrManager(std::make_shared<SharedPtrManager<T>>())
        {
        }

        Parent(const Parent &that) : sharedPtrManager(that.sharedPtrManager)
        {
        }

        Parent(Parent &&that) : sharedPtrManager(that.sharedPtrManager)
        {
        }
    };

    class AvXmlDocument;
    class AvXmlElement;
    using AvXmlElementRef = std::shared_ptr<AvXmlElement>;
    class AvXmlElement : public Parent<AvXmlElement>
    {
        friend AvXmlDocument;
        std::string m_sName;
        std::vector<std::pair<std::string, std::string>> m_vecAttributes;
        std::string m_sText;
        std::vector<AvXmlElementRef> m_vecChildren;
        bool m_bIsSelfClosing;

      public:
        AvXmlElement(const std::string &name);
        std::string toString() const;
        void setAttribute(const std::string &key, const std::string &value);
        void setAttribute(const std::string &key, size_t value);
        void removeAttribute(const std::string &key);
        void setText(std::string txt);
        void setSelfClose(bool close = true);
        AvXmlElementRef insertChildElement(std::string name);
        /* return true if successfully found */
        bool removeChildElement(std::string name);
        bool removeChildElement(AvXmlElementRef elem);
        std::vector<AvXmlElementRef> getChildren();
    };

    class AvXmlDocument : Parent<AvXmlElement>
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