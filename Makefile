MAIN := main
SRCS := main.cc XmlWriter.cpp
XML  := outputs.xml

$(MAIN): $(SRCS)
	clang++ $^ -o $@

$(XML): $(MAIN)
	./$^ $@

build: $(MAIN)

run: $(XML)

clean:
	rm -f $(XML) $(MAIN)

