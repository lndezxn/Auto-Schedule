# Auto-Schedule
 Automatically display upcoming courses and links
 
 To specify your own schedule, edit "course.txt" to modify a list of courses and it's link. Then edit "course_time.txt" to modify the time table using the template below.
 
 The existing "course.txt" and "course_time.txt" in the release zip shows an example of how to modify these files.
 
 ## Modify course.txt
 
 List your course and it's link in this format (the bracket is not included):
 
    [Course Name] [Link]
    
 The file should end with `end` in a single line.
 
 ## Modify course_time.txt
 
 List your time table in this format (the bracket is not included):
 
    [Course Name] [Day of week(1/2/3/4/5)] [Hour of start time] [Minute of start time] [Hour of end time] [Minute of end time]
 
The `[Course Name]` must appear in "course.txt", refering a subject. The file should end with `end` in a single line.

 Then, execute build.exe.
