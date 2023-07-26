using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Text.RegularExpressions;

namespace MakeHidden
{
    public partial class HiddenForm : Form
    {
        IgnoreUiChanges m_ignore = new IgnoreUiChanges();
        List<Variable> m_vars = new List<Variable>();
        string[] m_args;


        public HiddenForm(string[] args)
        {
            m_args = args;

            InitializeComponent();

            LoadFile();

            using (var ignore = m_ignore.Updating)
            {
                m_list.Items.Clear();

                m_list.Items.Add(new NewVariable());

                foreach (var cur in m_vars)
                {
                    m_list.Items.Add(cur);
                }

                m_name.Text = "";
                m_value.Text = "";

                EnableControls(false);

                Resort();
            }
        }

        void EnableControls(bool val)
        {
            m_name.Enabled = val;
            m_ascii.Enabled = val;
            m_wide.Enabled = val;
            m_value.Enabled = val;
            m_delete.Enabled = val;
        }

        void Delete_Click(object sender, EventArgs e)
        {
            using (var ignore = m_ignore.Updating)
            {

                m_vars.RemoveAt(m_list.SelectedIndex - 1);
                m_list.Items.RemoveAt(m_list.SelectedIndex);

                m_name.Text = "";
                m_value.Text = "";

                EnableControls(false);
            }

        }

        void Generate_Click(object sender, EventArgs e)
        {
            Resort();

            SaveFile();

            HashSet<string> used = new HashSet<string>();
            StringBuilder warnings = new StringBuilder();

            StringBuilder header = new StringBuilder();
            StringBuilder source = new StringBuilder();

            header.AppendLine("WCHAR* GetHiddenW(int index);");
            header.AppendLine("char* GetHiddenA(int index);");

            foreach (var cur in m_vars)
            {
                if (!used.Contains(cur.Name))
                {
                    used.Add(cur.Name);

                    if (cur.Wide || cur.Ascii)
                    {
                        header.AppendLine("#define STR_" + cur.Name + " " + used.Count);
                    }

                    if (cur.Wide)
                    {
                        header.AppendLine("#define HIDEW_" + cur.Name + " GetHiddenW(STR_" + cur.Name + ")");
                    }

                    if (cur.Ascii)
                    {
                        header.AppendLine("#define HIDEA_" + cur.Name + " GetHiddenA(STR_" + cur.Name + ")");
                    }
                }
                else
                {
                    warnings.AppendLine("WARNING: " + cur.Name + " is used more than once");
                }
            }

            source.AppendLine("#include \"main.h\"");
            source.AppendLine("#include \"Hidden.h\"");

            source.AppendLine("WCHAR* GetHiddenW(int index)");
            source.AppendLine("{");
            source.AppendLine("    WCHAR* buffer;");
            source.AppendLine("    CHAR* pad1;");
            source.AppendLine("    CHAR* pad2;");
            source.AppendLine("    int len = 0;");
            source.AppendLine("    int i = 0;");
            source.AppendLine("    switch (index)");
            source.AppendLine("    {");

            used.Clear();
            foreach (var cur in m_vars)
            {
                if (cur.Wide)
                {
                    if (!used.Contains(cur.Name))
                    {
                        used.Add(cur.Name);
                        Random rnd = new Random(2020 + cur.ID);

                        AppendComment(source, FixUp(cur));

                        source.AppendLine("    case STR_" + cur.Name + ":");
                        source.AppendLine("        {");
                        source.AppendLine("            static WCHAR localBuffer[" + (FixUp(cur).Length + 1) + "];");
                        source.AppendLine("            len = " + FixUp(cur).Length + ";");

                        string pad1 = "";
                        string pad2 = "";

                        GeneratePads(cur, rnd, ref pad1, ref pad2);

                        source.AppendLine("            pad1 = \"" + pad1 + "\";");
                        source.AppendLine("            pad2 = \"" + pad2 + "\";");
                        source.AppendLine("            buffer = localBuffer;");
                        source.AppendLine("        }");
                        source.AppendLine("        break;");
                    }
                }
            }

            source.AppendLine("    }");
            source.AppendLine("    for (i = 0; i < len; i ++)");
            source.AppendLine("    {");
            source.AppendLine("        buffer[i] = (char)(((unsigned char)pad1[i]) ^ ((unsigned char)pad2[i]));");
            source.AppendLine("    }");
            source.AppendLine("    buffer[len] = 0;");
            source.AppendLine("    return buffer;");
            source.AppendLine("}");

            source.AppendLine("char* GetHiddenA(int index)");
            source.AppendLine("{");
            source.AppendLine("    char* buffer;");
            source.AppendLine("    CHAR* pad1;");
            source.AppendLine("    CHAR* pad2;");
            source.AppendLine("    int len = 0;");
            source.AppendLine("    int i = 0;");
            source.AppendLine("    switch (index)");
            source.AppendLine("    {");

            used.Clear();
            foreach (var cur in m_vars)
            {
                if (cur.Ascii)
                {
                    if (!used.Contains(cur.Name))
                    {
                        used.Add(cur.Name);
                        Random rnd = new Random(4040 + cur.ID);

                        AppendComment(source, FixUp(cur));

                        source.AppendLine("    case STR_" + cur.Name + ":");
                        source.AppendLine("        {");
                        source.AppendLine("            static char localBuffer[" + (FixUp(cur).Length + 1) + "];");
                        source.AppendLine("            len = " + FixUp(cur).Length + ";");

                        string pad1 = "";
                        string pad2 = "";
                        GeneratePads(cur, rnd, ref pad1, ref pad2);

                        source.AppendLine("            pad1 = \"" + pad1 + "\";");
                        source.AppendLine("            pad2 = \"" + pad2 + "\";");
                        source.AppendLine("            buffer = localBuffer;");
                        source.AppendLine("        }");
                        source.AppendLine("        break;");
                    }
                }
            }

            source.AppendLine("    }");
            source.AppendLine("    for (i = 0; i < len; i ++)");
            source.AppendLine("    {");
            source.AppendLine("        buffer[i] = (char)(((unsigned char)pad1[i]) ^ ((unsigned char)pad2[i]));");
            source.AppendLine("    }");
            source.AppendLine("    buffer[len] = 0;");
            source.AppendLine("    return buffer;");
            source.AppendLine("}");

            used.Clear();

            if (warnings.Length > 0)
            {
                MessageBox.Show(warnings.ToString(), "Hidden Strings", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
            else
            {
                bool show = true;
                if (m_args != null && m_args.Length > 0)
                {
                    show = false;
                }

                if (show)
                {
                    MessageBox.Show("All done, files generated.", "Hidden Strings", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }

            File.WriteAllText(@"..\..\..\Hidden.h", header.ToString());
            File.WriteAllText(@"..\..\..\Hidden.cpp", source.ToString());
        }

        void GeneratePads(Variable cur, Random rnd, ref string pad1, ref string pad2)
        {
            int count1 = 0;
            int count2 = 0;

            foreach (var dig in FixUp(cur))
            {
                if (count1 >= 80)
                {
                    pad1 += "\"\r\n                   \"";
                    count1 = 0;
                }

                if (count2 >= 80)
                {
                    pad2 += "\"\r\n                   \"";
                    count2 = 0;
                }

                byte a = (byte)rnd.Next(0, 256);
                byte b = (byte)(((byte)dig) ^ a);

                pad1 += "\\x" + a.ToString("x2");
                count1 += 4;

                pad2 += "\\x" + b.ToString("x2");
                count2 += 4;
            }
        }

        void AppendComment(StringBuilder source, string value)
        {
            bool first = true;
            
            source.AppendLine();
            source.Append("        /* ");

            if (value.Length < 10)
            {
                value = value.Replace("/*", "/ *");
                value = value.Replace("*/", "* /");
                value = value.Replace("\n", "\\n");
                value = value.Replace("\r", "\\r");

                source.Append(value);
            }
            else
            {
                foreach (var line in value.Split('\n'))
                {
                    if (!first)
                    {
                        source.AppendLine();
                        source.Append("         * ");
                    }
                    source.Append(line.Replace("/*", "/ *").Replace("*/", "* /"));
                    first = false;
                }
            }

            source.AppendLine(" */");

        }

        void Resort()
        {
            m_vars.Sort((a, b) =>
                {
                    return a.Name.CompareTo(b.Name);
                });

            m_list.SelectedIndex = -1;

            for (int i = 0; i < m_vars.Count; i++)
            {
                m_list.Items[i + 1] = m_vars[i];
            }
        }

        string FixUp(Variable value)
        {
            if (value.IsSpecial)
            {
                return value.Value;
            }
            else
            {
                return Regex.Replace(value.Value, "\\$\\{(?<var>.*?)\\}", (m) =>
                    {
                        foreach (var cur in m_vars)
                        {
                            if (cur.Name == m.Groups["var"].Value)
                            {
                                return FixUp(cur);
                            }
                        }
                        throw new Exception();
                    }, RegexOptions.Singleline).Replace("\r", "");
            }
        }

        void List_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!m_ignore.Set)
            {
                using (var ignore = m_ignore.Updating)
                {
                    if (m_list.SelectedIndex == -1)
                    {
                        m_name.Text = "";
                        m_value.Text = "";
                        m_wide.Checked = false;
                        m_ascii.Checked = false;

                        EnableControls(false);
                    }
                    else if (m_list.SelectedItem is NewVariable)
                    {
                        m_name.Text = "NEWVARIABLE";
                        m_value.Text = "empty";
                        m_wide.Checked = false;
                        m_ascii.Checked = false;

                        EnableControls(true);
                        m_delete.Enabled = false;
                    }
                    else
                    {
                        Variable var = (Variable)m_list.SelectedItem;

                        m_name.Text = var.Name;
                        m_value.Text = var.ValueRaw;
                        m_wide.Checked = var.Wide;
                        m_ascii.Checked = var.Ascii;

                        EnableControls(true);
                    }
                }
            }
        }

        void Name_TextChanged(object sender, EventArgs e)
        {
            UpdateVar();
        }

        void UpdateVar()
        {
            if (!m_ignore.Set)
            {
                using (var ignore = m_ignore.Updating)
                {
                    if (m_list.SelectedItem is NewVariable)
                    {
                        Variable var = new Variable();
                        var.Name = m_name.Text;
                        var.ValueRaw = m_value.Text;
                        var.Ascii = m_ascii.Checked;
                        var.Wide = m_wide.Checked;
                        var.ID = CreateID();

                        m_list.SelectedIndex = m_list.Items.Add(var);
                        m_vars.Add(var);

                        m_delete.Enabled = true;
                    }
                    else
                    {
                        Variable var = (Variable)m_list.SelectedItem;
                        var.Name = m_name.Text;
                        var.ValueRaw = m_value.Text;
                        var.Ascii = m_ascii.Checked;
                        var.Wide = m_wide.Checked;
                        m_list.Items[m_list.SelectedIndex] = var;
                    }
                }
            }
        }

        void Value_TextChanged(object sender, EventArgs e)
        {
            UpdateVar();
        }

        string DataFile
        {
            get
            {
                return @"..\..\HiddenData.txt";
            }
        }

        void LoadFile()
        {
            if (File.Exists(DataFile))
            {
                string data = File.ReadAllText(DataFile);

                int count = ReadNumber(ref data);
                for (int i = 0; i < count; i++)
                {
                    Variable var = new Variable();
                    var.ID = ReadNumber(ref data);
                    var.Name = ReadString(ref data);
                    var.ValueRaw = ReadString(ref data);

                    string temp = ReadString(ref data);

                    if (temp.Contains("A"))
                    {
                        var.Ascii = true;
                    }

                    if (temp.Contains("W"))
                    {
                        var.Wide = true;
                    }
                    
                    m_vars.Add(var);
                }
            }
        }

        int CreateID()
        {
            HashSet<int> ids = new HashSet<int>();

            foreach (var cur in m_vars)
            {
                ids.Add(cur.ID);
            }

            for (int i = 0; ; i++)
            {
                if (!ids.Contains(i))
                {
                    return i;
                }
            }

            throw new Exception();
        }

        int ReadNumber(ref string data)
        {
            int ret = int.Parse(data.Substring(0, 6));
            data = data.Substring(8);
            return ret;
        }

        string ReadString(ref string data)
        {
            int len = ReadNumber(ref data);
            string ret = data.Substring(0, len);
            data = data.Substring(len + 2);
            return ret;
        }

        void HiddenForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            SaveFile();
        }

        void SaveFile()
        {
            StringBuilder sb = new StringBuilder();
            AppendNumber(sb, m_vars.Count);

            foreach (var cur in m_vars)
            {
                AppendNumber(sb, cur.ID);
                AppendString(sb, cur.Name);
                AppendString(sb, cur.ValueRaw);
                AppendString(sb, (cur.Ascii ? "A" : "") + (cur.Wide ? "W" : ""));
            }

            if (File.Exists(DataFile))
            {
                File.Delete(DataFile);
            }

            File.WriteAllText(DataFile, sb.ToString());
        }

        void AppendString(StringBuilder sb, string value)
        {
            AppendNumber(sb, value.Length);
            sb.Append(value);
            sb.Append("\r\n");
        }

        void AppendNumber(StringBuilder sb, int value)
        {
            sb.Append(value.ToString("000000"));
            sb.Append("\r\n");
        }

        void ExitMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        void NewItemMenuItem_Click(object sender, EventArgs e)
        {
            m_list.SelectedIndex = 0;
            m_name.Select();
        }

        void Ascii_CheckedChanged(object sender, EventArgs e)
        {
            UpdateVar();
        }

        void Wide_CheckedChanged(object sender, EventArgs e)
        {
            UpdateVar();
        }

        void UpVersion()
        {
            foreach (var cur in m_vars)
            {
                if (cur.Name == "VERSION")
                {
                    cur.ValueRaw = m_args[0];
                }
            }

            Generate_Click(null, null);
            Close();
        }

        void HiddenForm_Load(object sender, EventArgs e)
        {
            if (m_args != null && m_args.Length == 1)
            {
                BeginInvoke(new MethodInvoker(UpVersion));
            }
        }
    }
}
