using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MakeHidden
{
    class Variable
    {
        public string Name;
        public string ValueRaw;
        public bool Ascii;
        public bool Wide;

        public bool IsSpecial
        {
            get
            {
                if (Name == "LINEFEED" ||
                    Name == "NEWLINE")
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }


        public string Value
        {
            get
            {
                if (Name == "LINEFEED")
                {
                    return "\n";
                }
                else if (Name == "NEWLINE")
                {
                    return "\r\n";
                }
                else
                {
                    return ValueRaw;
                }
            }
        }

        public int ID;

        public override string ToString()
        {
            return Name;
        }
    }
}
