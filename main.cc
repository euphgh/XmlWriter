#include "XmlWriter.h"

int main() {
  auto doc = XmlWriter::XmlDoc("bia_repeater");
  auto bia_repeater = doc.rootTag();
  bia_repeater.setAttribute("normal", "rep");
  auto foo = bia_repeater.insertChildTag("foo");
  auto bar = bia_repeater.insertChildTag("bar");
  foo.setAttribute("hello", "world");
  doc.write("outputs.xml");
  return 0;
}