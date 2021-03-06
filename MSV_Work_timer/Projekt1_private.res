L            .rsrc           �  <   �         @  �    �dO          @  �   �  �	   �  �
   �  �     �   P �    �dO          `  �   x  �    �dO           �      �dO           �      �dO       �  �  �    �dO           �      �dO       � ��  �    �dO                 �dO       � � �    �dO                 �dO       � �8 �    �dO           $      �dO          h �    �dO           4   I D C _ K E Y B O A R D _ S H O R T C U T S  I D B _ S T Y L E _ X S L  I C O N D             D  @          �  �                         z          �  "           �  �          (       @                                  �  �   �� �   � � ��  ��� ���   �  �   �� �   � � ��  ���                     ����������     ww�wwwwwwwx    x�������x    �����w�   ������������p   �������������   ����������p     ��30����p     ��?������p     ����0��p     ��?�����p     ��� �p     ���� ����p     ���F�@�p     ���l���p     �����F�@�p     ��w�tl�0p     �� �F�@0p     �������l�      �������F�D0     ���w����C     ��� ��F�D0    ���������l�C    ���������F�D0�ww��w�w���l�C����� � ���F�@�w����������tl�������������� D@��x����������   ��wwwwwwwww�    �����������    �� �  �  �  �   �   �   �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �                     ?�  �  �                        (                �                         �  �   �� �   � � ��  ��� ���   �  �   �� �   � � ��  ���           ��www  �����p ������ ���p  �3?wp  ���p  ��Lp  ����p  ���L   ���  �����L ���t�������pD������  �����  ������� ��� ��������������������������� ��  �� �����                        �Ȑ         � W     R e l e a s e   h o u r s    M s   S h e l l   D l g      P    ' ? 2  ���� Y e s       P    e ? 2  ���� C a n c e l        P    	  :  ���� E n d   o f   w e e k      P    	 / :  ���� E n d   o f   m o n t h         P      � I ���� D o   y o u   r e a l l y   w a n t   r e l e a s e   h o u r s ?        P    O     ���� 0 : 0 0 : 0 0     � E   <?xml version='1.0'?>
<xsl:stylesheet
	version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:template match="WT_SUMMARY">
		<html>
			<head>
				<title> Work timer </title>
			</head>

			<body bgcolor="#e0e0f0">
				<xsl:apply-templates/>
			</body>
		</html>
	</xsl:template>

	<xsl:template match="WT_LISTING">
		<p/>
		<div align="center">
			<h2> Summary of time registration </h2>
		</div>
		<table cols="4" width="90%" align="center">
			<tr>
				<td width="25%"> </td>
				<td width="25%"> </td>
				<td width="25%"> </td>
				<td width="25%"> </td>
			</tr>
			<xsl:apply-templates/>
		</table>
	</xsl:template>

	<xsl:template match="WT_DAY_SUMMARY">
		<p/>
		<table width="90%" rows="7" align="center">
    <xsl:for-each select="WT_HEADER">
			<tr>
        <th width="16%" bgcolor="#dddda0" align="center"> Date </th>
				<th width="12%" bgcolor="#ffffc0" align="center"> Daily delta </th>
				<th width="12%" bgcolor="#ffffc0" align="center"> Global delta </th>
				<th width="12%" bgcolor="#9999CC" align="center"> <xsl:value-of select="H_TIMER_1" /> </th>
				<th width="12%" bgcolor="#AAAAEE" align="center"> <xsl:value-of select="H_TIMER_2" /> </th>
				<th width="12%" bgcolor="#9999CC" align="center"> <xsl:value-of select="H_TIMER_3" /> </th>
				<th width="12%" bgcolor="#AAAAEE" align="center"> <xsl:value-of select="H_TIMER_4" /> </th>
        <th width="12%" bgcolor="#9999CC" align="center"> <xsl:value-of select="H_TIMER_5" /> </th>
			</tr>
     </xsl:for-each>
			<xsl:for-each select="WT_RECORD">
				<tr align="center" bgcolor="lightgreen">
          <td> <xsl:value-of select="DATE" /> </td>
					<td> <xsl:value-of select="DAILY_DELTA" /> </td>
					<td> <xsl:value-of select="GLOBAL_DELTA" /> </td>
					<td bgcolor="#99FFCC"> <xsl:value-of select="TIMER_1" /> </td>
					<td bgcolor="#99FFFF"> <xsl:value-of select="TIMER_2" /> </td>
					<td bgcolor="#99FFCC"> <xsl:value-of select="TIMER_3" /> </td>
					<td bgcolor="#99FFFF"> <xsl:value-of select="TIMER_4" /> </td>
          <td bgcolor="#99FFCC"> <xsl:value-of select="TIMER_5" /> </td>
				</tr>
			</xsl:for-each>
		</table>
	</xsl:template>

</xsl:stylesheet>
                  @     �4   V S _ V E R S I O N _ I N F O     ���     N    N                             8    S t r i n g F i l e I n f o       0 8 0 9 0 4 E 4   (   C o m p a n y N a m e     J S P   ,   F i l e V e r s i o n     2 . 4 . 2   X   F i l e D e s c r i p t i o n     T i m e r   f o r   w o r k   o r g a n i z e   "   I n t e r n a l N a m e       @   L e g a l C o p y r i g h t   T o m a s z   J o k i e l   X   L e g a l T r a d e m a r k s     t o m a s z . j o k i e l @ g m a i l . c o m   *   O r i g i n a l F i l e n a m e       6   P r o d u c t N a m e     W o r k   T i m e r     0   P r o d u c t V e r s i o n   2 . 4 . 2   D     V a r F i l e I n f o     $    T r a n s l a t i o n     	��       �       �                     $       4       .rsrc              