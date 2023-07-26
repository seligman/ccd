namespace MakeHidden
{
    partial class HiddenForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.m_list = new System.Windows.Forms.ListBox();
            this.m_name = new System.Windows.Forms.TextBox();
            this.m_value = new System.Windows.Forms.TextBox();
            this.m_delete = new System.Windows.Forms.Button();
            this.m_generate = new System.Windows.Forms.Button();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newItemToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.m_wide = new System.Windows.Forms.CheckBox();
            this.m_ascii = new System.Windows.Forms.CheckBox();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_list
            // 
            this.m_list.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.m_list.Font = new System.Drawing.Font("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_list.FormattingEnabled = true;
            this.m_list.IntegralHeight = false;
            this.m_list.ItemHeight = 16;
            this.m_list.Location = new System.Drawing.Point(12, 27);
            this.m_list.Name = "m_list";
            this.m_list.Size = new System.Drawing.Size(203, 379);
            this.m_list.TabIndex = 0;
            this.m_list.SelectedIndexChanged += new System.EventHandler(this.List_SelectedIndexChanged);
            // 
            // m_name
            // 
            this.m_name.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_name.Font = new System.Drawing.Font("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_name.Location = new System.Drawing.Point(221, 27);
            this.m_name.Name = "m_name";
            this.m_name.Size = new System.Drawing.Size(425, 22);
            this.m_name.TabIndex = 1;
            this.m_name.TextChanged += new System.EventHandler(this.Name_TextChanged);
            // 
            // m_value
            // 
            this.m_value.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_value.Font = new System.Drawing.Font("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_value.Location = new System.Drawing.Point(221, 55);
            this.m_value.Multiline = true;
            this.m_value.Name = "m_value";
            this.m_value.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.m_value.Size = new System.Drawing.Size(425, 322);
            this.m_value.TabIndex = 2;
            this.m_value.WordWrap = false;
            this.m_value.TextChanged += new System.EventHandler(this.Value_TextChanged);
            // 
            // m_delete
            // 
            this.m_delete.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.m_delete.Location = new System.Drawing.Point(221, 383);
            this.m_delete.Name = "m_delete";
            this.m_delete.Size = new System.Drawing.Size(75, 23);
            this.m_delete.TabIndex = 3;
            this.m_delete.Text = "Delete";
            this.m_delete.UseVisualStyleBackColor = true;
            this.m_delete.Click += new System.EventHandler(this.Delete_Click);
            // 
            // m_generate
            // 
            this.m_generate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.m_generate.Location = new System.Drawing.Point(571, 383);
            this.m_generate.Name = "m_generate";
            this.m_generate.Size = new System.Drawing.Size(75, 23);
            this.m_generate.TabIndex = 6;
            this.m_generate.Text = "Generate";
            this.m_generate.UseVisualStyleBackColor = true;
            this.m_generate.Click += new System.EventHandler(this.Generate_Click);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(658, 24);
            this.menuStrip1.TabIndex = 7;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newItemToolStripMenuItem,
            this.toolStripMenuItem1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // newItemToolStripMenuItem
            // 
            this.newItemToolStripMenuItem.Name = "newItemToolStripMenuItem";
            this.newItemToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
            this.newItemToolStripMenuItem.Size = new System.Drawing.Size(177, 22);
            this.newItemToolStripMenuItem.Text = "New item...";
            this.newItemToolStripMenuItem.Click += new System.EventHandler(this.NewItemMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(174, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(177, 22);
            this.exitToolStripMenuItem.Text = "E&xit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.ExitMenuItem_Click);
            // 
            // m_wide
            // 
            this.m_wide.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.m_wide.AutoSize = true;
            this.m_wide.Location = new System.Drawing.Point(514, 387);
            this.m_wide.Name = "m_wide";
            this.m_wide.Size = new System.Drawing.Size(51, 17);
            this.m_wide.TabIndex = 5;
            this.m_wide.Text = "Wide";
            this.m_wide.UseVisualStyleBackColor = true;
            this.m_wide.CheckedChanged += new System.EventHandler(this.Wide_CheckedChanged);
            // 
            // m_ascii
            // 
            this.m_ascii.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.m_ascii.AutoSize = true;
            this.m_ascii.Location = new System.Drawing.Point(455, 387);
            this.m_ascii.Name = "m_ascii";
            this.m_ascii.Size = new System.Drawing.Size(53, 17);
            this.m_ascii.TabIndex = 4;
            this.m_ascii.Text = "ASCII";
            this.m_ascii.UseVisualStyleBackColor = true;
            this.m_ascii.CheckedChanged += new System.EventHandler(this.Ascii_CheckedChanged);
            // 
            // HiddenForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(658, 418);
            this.Controls.Add(this.m_ascii);
            this.Controls.Add(this.m_wide);
            this.Controls.Add(this.m_generate);
            this.Controls.Add(this.m_delete);
            this.Controls.Add(this.m_value);
            this.Controls.Add(this.m_name);
            this.Controls.Add(this.m_list);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "HiddenForm";
            this.Text = "Hidden Strings";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.HiddenForm_FormClosing);
            this.Load += new System.EventHandler(this.HiddenForm_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox m_list;
        private System.Windows.Forms.TextBox m_name;
        private System.Windows.Forms.TextBox m_value;
        private System.Windows.Forms.Button m_delete;
        private System.Windows.Forms.Button m_generate;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newItemToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.CheckBox m_wide;
        private System.Windows.Forms.CheckBox m_ascii;
    }
}

