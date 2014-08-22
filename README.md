ArithmeticRecognitionSoftware
=============================

cs283 final project. Recognizes and evaluates simple equations.


Compiling
=============================

mkdir libs
make


Running
=============================

./finalex test_images/eq10.png

Purpose
=============================

THe program takes an image with an expression as input and then attempts to 
calculate the result.

How this is complete is the image will be converted to black and white to minimize
background distortions. Then, the image is cropped into individual images containing
a symbol or digit. Each image is then analyzed to come up with the closest match and
the result will be a string containing the expression. The string is then converted
from infix to postfix to take into account precedence and parenthesis. Once it is 
in postfix, the expression is evaluated and the final result is outputted onto the screen.


Notes
=============================

After running the program, the saves folder will be populated with the
various steps of the identification process, such as making it black and
white, splitting the image, and resizing the subimages. See this if you're
interested. The folder will contain images and results of past runs/tests
so you can look at our recent history of testing. They will be overwritten
if you run the program again. 


Ideally, the image should have little to no shadows to minimize image distortion
and the program is not 100 percent accurate. Though, if the program adheres to
ideal conditions, more often than not, it will evaluate the expression correctly. 
