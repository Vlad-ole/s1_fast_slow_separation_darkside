void parse()
{
    const string file_name = "/home/darkside/Vlad_Programs/test_parse.txt";

    ifstream input_file;
    input_file.open(file_name.c_str());
    if(!input_file.is_open())
    {
        cout << "error in file.open = " << file_name << endl;
        exit(1);
    }

    string line;

    while (std::getline(input_file, line))
    {
        if (line[0] != "#" )
        {
            std::istringstream iss(line);
            float x, y, z; // The number in the line

            //while the iss is a number
            while ((iss >> x >> y >> z))
            {
                //look at the number
                cout << x << " " << y << " " << z << endl;
            }
        }
    }

}
