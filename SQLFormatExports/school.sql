CREATE TABLE IF NOT EXISTS "Grades" (
     "Index"	VARCHAR(50) NOT NULL,
   "SubName"	VARCHAR(50) NOT NULL,
     "Grade"	VARCHAR(50) NOT NULL
);
CREATE TABLE IF NOT EXISTS "Professor" (
   "ProfessorID"	VARCHAR(50) NOT NULL,
          "Name"	VARCHAR(50) NOT NULL,
       "Surname"	VARCHAR(50) NOT NULL,
           "Age"	VARCHAR(50) NOT NULL
);
CREATE TABLE IF NOT EXISTS "Student" (
        "ID"	VARCHAR(50) NOT NULL,
      "Name"	VARCHAR(50) NOT NULL,
   "Surname"	VARCHAR(50) NOT NULL
);
INSERT INTO "Grades" VALUES ('192','Fizika','7');
INSERT INTO "Grades" VALUES ('589','Fizika','7');
INSERT INTO "Grades" VALUES ('175','Fizika','7');
INSERT INTO "Professor" VALUES ('100','Tatjana','Lutovac','48');
INSERT INTO "Student" VALUES ('192','Filip','Petrovic');
INSERT INTO "Student" VALUES ('589','Jovan','Mosurovic');
INSERT INTO "Student" VALUES ('175','Lazar','Stankovic');
INSERT INTO "Student" VALUES ('445','Ilija','Jankovic');
