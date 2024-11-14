#include "AvXmlDocument.h"

int main(int argc, char *argv[])
{
    auto doc = Connectivity::AvXmlDocument("bia_repeater");
    auto bia_repeater = doc.getRootElement();
    bia_repeater.setAttribute("normal", "rep");
    auto foo = bia_repeater.insertChildElement("foo");
    auto bar = bia_repeater.insertChildElement("bar");
    foo.setAttribute("hello", "world");
    doc.saveFile(argv[1]);
    return 0;
}