#ifndef __XMLWRITER_H__
#define __XMLWRITER_H__

#include <cstddef>
#include <string>
#include <vector>

namespace XmlWriter {

class XmlDoc;

class NodeRef {
private:
  size_t index;
  XmlDoc &doc;

  friend class XmlDoc;
  friend class XmlNode;

public:
  NodeRef(size_t index, XmlDoc &doc) : index(index), doc(doc) {}
  void setAttribute(const std::string &key, const std::string &value);
  void setText(std::string txt);
  void setClose(bool close = true);
  NodeRef insertChildTag(std::string name);

  const std::string name() const;
};

class XmlNode {
  std::string name;
  std::vector<std::pair<std::string, std::string>> attributes;
  std::string text;
  std::vector<NodeRef> childen;
  bool selfClose;

  friend class NodeRef;
  friend class XmlDoc;

public:
  XmlNode(const std::string &name);
  std::string toString();
};

class XmlDoc {
  std::vector<XmlNode> pool;

  friend class NodeRef;

public:
  XmlDoc(const std::string &rootName);
  void printNode(std::ofstream &out, const NodeRef &curr, int deep);
  int write(const std::string &fileName);
  void show();
  NodeRef rootTag();
};

} // namespace XmlWriter

#endif