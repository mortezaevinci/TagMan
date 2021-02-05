<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.pTag = New System.Windows.Forms.PictureBox()
        Me.bOpen = New System.Windows.Forms.Button()
        Me.OpenDialog1 = New System.Windows.Forms.OpenFileDialog()
        Me.lMessage = New System.Windows.Forms.Label()
        CType(Me.pTag, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'pTag
        '
        Me.pTag.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.pTag.Location = New System.Drawing.Point(12, 41)
        Me.pTag.Name = "pTag"
        Me.pTag.Size = New System.Drawing.Size(349, 409)
        Me.pTag.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage
        Me.pTag.TabIndex = 0
        Me.pTag.TabStop = False
        '
        'bOpen
        '
        Me.bOpen.Location = New System.Drawing.Point(12, 12)
        Me.bOpen.Name = "bOpen"
        Me.bOpen.Size = New System.Drawing.Size(75, 23)
        Me.bOpen.TabIndex = 1
        Me.bOpen.Text = "Open..."
        Me.bOpen.UseVisualStyleBackColor = True
        '
        'OpenDialog1
        '
        Me.OpenDialog1.FileName = "image"
        Me.OpenDialog1.InitialDirectory = "Q:\My files\Project HandTag\sample pictures\tags2"
        '
        'lMessage
        '
        Me.lMessage.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.lMessage.Location = New System.Drawing.Point(380, 41)
        Me.lMessage.Name = "lMessage"
        Me.lMessage.Size = New System.Drawing.Size(335, 409)
        Me.lMessage.TabIndex = 2
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(727, 462)
        Me.Controls.Add(Me.lMessage)
        Me.Controls.Add(Me.bOpen)
        Me.Controls.Add(Me.pTag)
        Me.Name = "Form1"
        Me.Text = "Form1"
        CType(Me.pTag, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents pTag As System.Windows.Forms.PictureBox
    Friend WithEvents bOpen As System.Windows.Forms.Button
    Friend WithEvents OpenDialog1 As System.Windows.Forms.OpenFileDialog
    Friend WithEvents lMessage As System.Windows.Forms.Label

End Class
