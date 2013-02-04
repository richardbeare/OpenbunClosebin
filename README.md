OpenbunClosebin
===============

Morphological opening/closing by union/intersection of line structuring elements

OpenBun - Opening by union (pixel-wise maxima) of openings by many line
segments at selectable orientations.

CloseBin - Closing by intersection (pixel-wise minima) of closings by
many line segments at selectable orientations.

These filters are intended to remove structures based on the length of
a line that will fit inside the structure. For example, closebin can
be used to remove dark blobs while keeping dark lines. In addition,
these filters can also be used to remove thin, but curving structures
while retaining very large blobs which have complex edges.