# char_filter
simple character filter

This program positively or negatively filters ascii characters byte-by-byte in a file or stdin stream by various categories (e.g. numeric, alphanumeric, printable) or via a provided string of a set of ascii characters. This will not be semantically defined by regular expressions. 

I'm probably replicating the exact functionality of an existing unix program that has escaped my memory and lazy attempts to google, but often the function of this program is done using sed, awk, perl oneliners, or tr (with tr -d actually replicating a large amount of what I intend with this program), and that seems rather clunky to me, both in syntax and in performance.  cat foo | cf -a  feels better than cat foo | sed 's/[^[:alpha:]]//g', and will probably use even less resources.
