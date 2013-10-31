#!/usr/bin/env io_static 

i := 1
m := 2147483647 # INT_MAX
w := 70

lines := File standardInput readLines 

nodes     := List clone
node2name := Map  clone 

keys      := List clone 
key2name  := Map  clone 

results   := List clone 
r2name    := Map  clone 

lines foreach (line, 
	       words := line split 
	       if (words at(1) == "ID:", 
			 nodes append (words at(3) asNumber)
			 node2name atPut(words at(3), words at(2)))
	       if (words at(1) == "TRANSLATE:", 
			 keys append (words at(3) asNumber)
			 key2name atPut(words at(3), words at(2)))
	       if (words at(1) == "RESULT:", 
			 results append (words at(3) asNumber)
			 r2name atPut(words at(3), words at(2)))
	       )

nodes sortInPlace

writeln("nodes")
80 repeat(write("-"))
writeln

space := Map clone 
nodes foreach (n, 
	       while (i / w < n / m, 
			write(" ")
			i = i+1)
	       write("|")
	       i = i+1
	       space atPut(n asString, i-2))
writeln 

nodes foreach(n, 
	      space at(n asString) repeat (write(" "))
	      writeln(node2name at(n asString)))

writeln("keys")

keys foreach(k, 
	     i := 0
	     while (i / w < k / m, 
		      write(" ")
		      i = i+1)
	     writeln(key2name at (k asString))
	     )

writeln("assignments")

results foreach(r, 
		i := 0
		while (i / w < r / m, 
			 write(" ")
			 i = i+1)
		writeln(r2name at (r asString))
		)

