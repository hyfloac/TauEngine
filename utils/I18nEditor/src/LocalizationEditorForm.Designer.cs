namespace I18nEditor
{
    partial class LocalizationEditorForm
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
            if(disposing && (components != null))
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
            System.Windows.Forms.Label searchLabel;
            this.menu = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.searchBox = new System.Windows.Forms.TextBox();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.localizationsTable = new System.Windows.Forms.DataGridView();
            this.unlocalizedNameColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.localizedNameColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            searchLabel = new System.Windows.Forms.Label();
            this.menu.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.localizationsTable)).BeginInit();
            this.SuspendLayout();
            // 
            // searchLabel
            // 
            searchLabel.AutoSize = true;
            searchLabel.Location = new System.Drawing.Point(12, 35);
            searchLabel.Name = "searchLabel";
            searchLabel.Size = new System.Drawing.Size(44, 13);
            searchLabel.TabIndex = 0;
            searchLabel.Text = "Search:";
            // 
            // menu
            // 
            this.menu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menu.Location = new System.Drawing.Point(0, 0);
            this.menu.Name = "menu";
            this.menu.Size = new System.Drawing.Size(800, 24);
            this.menu.TabIndex = 1;
            this.menu.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.saveToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.openToolStripMenuItem.Text = "Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // searchBox
            // 
            this.searchBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.searchBox.Location = new System.Drawing.Point(62, 32);
            this.searchBox.MaxLength = 1024;
            this.searchBox.Name = "searchBox";
            this.searchBox.Size = new System.Drawing.Size(726, 20);
            this.searchBox.TabIndex = 2;
            this.searchBox.WordWrap = false;
            // 
            // openFileDialog
            // 
            this.openFileDialog.Filter = "Lang Files (*.lang)|*.lang|Binary Lang Files (*.blang)|*.blang|All Files (*.*)|*." +
    "*";
            this.openFileDialog.Title = "Open Localization File";
            // 
            // saveFileDialog
            // 
            this.saveFileDialog.DefaultExt = "blang";
            this.saveFileDialog.Filter = "Lang Files (*.lang)|*.lang|Binary Lang Files (*.blang)|*.blang|All Files (*.*)|*." +
    "*";
            this.saveFileDialog.Title = "Save Translation File";
            // 
            // localizationsTable
            // 
            this.localizationsTable.AllowUserToOrderColumns = true;
            this.localizationsTable.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.localizationsTable.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.unlocalizedNameColumn,
            this.localizedNameColumn});
            this.localizationsTable.Location = new System.Drawing.Point(12, 58);
            this.localizationsTable.Name = "localizationsTable";
            this.localizationsTable.Size = new System.Drawing.Size(776, 380);
            this.localizationsTable.TabIndex = 3;
            // 
            // unlocalizedNameColumn
            // 
            this.unlocalizedNameColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.unlocalizedNameColumn.HeaderText = "Unlocalized Name";
            this.unlocalizedNameColumn.MaxInputLength = 1024;
            this.unlocalizedNameColumn.Name = "unlocalizedNameColumn";
            // 
            // localizedNameColumn
            // 
            this.localizedNameColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.localizedNameColumn.HeaderText = "Localized Name";
            this.localizedNameColumn.MaxInputLength = 1024;
            this.localizedNameColumn.Name = "localizedNameColumn";
            // 
            // LocalizationEditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.localizationsTable);
            this.Controls.Add(this.searchBox);
            this.Controls.Add(searchLabel);
            this.Controls.Add(this.menu);
            this.MainMenuStrip = this.menu;
            this.Name = "LocalizationEditorForm";
            this.Text = "LocalizationEditorForm";
            this.menu.ResumeLayout(false);
            this.menu.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.localizationsTable)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.MenuStrip menu;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.TextBox searchBox;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
        private System.Windows.Forms.SaveFileDialog saveFileDialog;
        private System.Windows.Forms.DataGridView localizationsTable;
        private System.Windows.Forms.DataGridViewTextBoxColumn unlocalizedNameColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn localizedNameColumn;
    }
}