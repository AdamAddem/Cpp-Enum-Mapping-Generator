## C++23 Enum <-> String Mapping Generator
The first line of the input file must contain <br>
<output_filename> <enum_name> <namespace> <underlying_type> <br>
All following lines will contain a pairing of the enum's name and its string representation <br>
Ex: ```PLUS +``` <br>
The first leading ! in a string representation will be stripped, so empty string reps can be specified with a single !<br>
To specify a category, use ```!BEGIN <category_name>``` <br>
Any following pairs will now be assigned to a category. Categories can overlap. <br>
End the category with ```!END <category_name>``` <br>
Each category will have a corresponding function with an enum as a parameter, returning whether that enum is part of the specified category. <br>
They will also have a macro defining a case for each enum within the category such that they would all fall through to the next statement within a switch. <br>
