# src

Some of these directories come from entire outside repositories brought in
using git-subtree. This means that the source files are inserted directly
into the mtchaind repository. They can be edited and committed just as if they
were normal files.

However, if you create a commit that contains files both from a
subtree, and from the mtchain source tree please use care when designing
the commit message, since it will appear in the subtree's individual
repository when the changes are pushed back to the upstream.

When submitting pull request, make sure that any commits which include
files from subtrees are isolated - i.e. do not mix files from subtrees
and mtchain in the same commit. This way, the commit message will make
sense. We don't want to see "Fix pathfinding bug with M" appearing
in the LevelDB or Beast commit log, for example.

About git-subtree:

https://github.com/apenwarr/git-subtree <br>
http://blogs.atlassian.com/2013/05/alternatives-to-git-submodule-git-subtree/ <br>

<table align=left><tr>
<th>dir</th>
<th>What</th>
</tr><tr>
<td>beast</td>
<td>Beast, the amazing cross-platform library.<br>
    git@github.com:vinniefalco/Beast.git
</td>
</tr></table>

## ./beast

Beast, the amazing cross-platform library.

Repository <br>
```
git@github.com:vinniefalco/Beast.git
```
Branch
```
master
```

## HyperLevelDB

MTChain's fork of HyperLevelDB

Repository <br>
```
git@github.com:mtchain/HyperLevelDB.git
```
Branch
```
mtchain-fork
```

## LevelDB

MTChain's fork of LevelDB.

Repository <br>
```
git@github.com:mtchain/LevelDB.git
```
Branch
```
mtchain-fork
```

## LightningDB (a.k.a. MDB)

MTChain's fork of MDB, a fast memory-mapped key value database system.

Repository <br>
```
git@github.com:mtchain/LightningDB.git
```
Branch
```
mtchain-fork
```

## websocket

MTChain's fork of websocketpp has some incompatible changes and MTChain specific includes.

Repository
```
git@github.com:mtchain/websocketpp.git
```
Branch
```
mtchain-fork
```

## protobuf

MTChain's fork of protobuf. We've changed some names in order to support the
unity-style of build (a single .cpp added to the project, instead of
linking to a separately built static library).

Repository
```
git@github.com:mtchain/protobuf.git
```
Branch
```
master
```

**NOTE** Linux builds use the protobuf installed in /usr/lib. This will be
fixed in a future revision.

## SQLite

Not technically a subtree but included here because it is a direct
copy of the official SQLite distributions available here:

http://sqlite.org/download.html
