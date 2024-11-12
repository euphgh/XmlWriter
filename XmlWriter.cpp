#include "XmlWriter.h"
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace XmlWriter {

using std::ofstream;
using std::string;

XmlNode::XmlNode(const std::string &name) : name(name), selfClose(true) {}

void NodeRef::setAttribute(const string &key, const string &value) {
  doc.pool[index].attributes.emplace_back(key, value);
}

void NodeRef::setText(string text) {
  doc.pool[index] = text;
  if (text != "")
    doc.pool[index].selfClose = false;
}

void NodeRef::setClose(bool close) { doc.pool[index].selfClose = close; }

string XmlNode::toString() {
  std::stringstream res("");
  res << "{ name = " << name;
  res << ", selfClose = " << selfClose;

  std::stringstream childRes("");
  childRes << "{ ";
  for (size_t i = 0; i < childen.size(); i++) {
    childRes << childen[i].index << ", ";
  }
  childRes << "}";

  res << ", children = " << childRes.str().c_str();

  std::stringstream attrRes("");
  attrRes << "{ ";
  for (size_t i = 0; i < attributes.size(); i++) {
    attrRes << attributes[i].first << " = " << attributes[i].second << ", ";
  }
  attrRes << "}";
  res << ", attr = " << attrRes.str().c_str() << " }";

  return res.str().c_str();
}

NodeRef NodeRef::insertChildTag(string name) {
  doc.pool.emplace_back(name);
  doc.pool[index].selfClose = false;
  doc.pool[index].childen.emplace_back(doc.pool.size() - 1, doc);
  return NodeRef(doc.pool.size() - 1, doc);
}

void XmlDoc::printNode(ofstream &out, const NodeRef &ref, int deep) {
  const auto curr = pool[ref.index];
  out << string(deep * 2, ' ') << '<' << curr.name;
  for (const auto &[key, value] : curr.attributes) {
    out << ' ' << key << "=\"" << value << "\"";
  }
  if (curr.selfClose) { // no text not child
    out << "/>\n";
  } else {
    out << '>' << curr.text << '\n';
    for (const auto child : curr.childen) {
      printNode(out, child, deep + 1);
    }
    out << "</" << curr.name << ">\n";
  }
}

XmlDoc::XmlDoc(const string &rootName) : pool({rootName}) {}

int XmlDoc::write(const string &fileName) {
  ofstream out(fileName);
  if (!out) {
    return 1;
  }
  printNode(out, rootTag(), 0);
  return 0;
}

void XmlDoc::show() {
  for (size_t i = 0; i < pool.size(); i++) {
    printf("node[%zu]: %s\n", i, pool[i].toString().c_str());
  }
}

NodeRef XmlDoc::rootTag() { return NodeRef(0, *this); }
} // namespace XmlWriter
