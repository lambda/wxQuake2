rsync -r doc/html/ emk@mccay.dartmouth.edu:public_html/bsp/
(cd ..; rsync -r doc/html/ emk@mccay.dartmouth.edu:public_html/quake2/)
ssh emk@mccay.dartmouth.edu 'cd public_html/bsp/; ./installdox -l Q2DOXYTAGS@../quake2'
