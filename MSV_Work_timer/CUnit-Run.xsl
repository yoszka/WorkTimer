<?xml version='1.0'?>
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

	<xsl:template match="CUNIT_RESULT_LISTING">
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

	<xsl:template match="CUNIT_RUN_SUITE">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="SUITE_NAME">
	</xsl:template>

	<xsl:template match="CUNIT_RUN_SUITE_SUCCESS">
		<tr bgcolor="#f0e0f0">
			<td colspan="4">
				Running Suite <xsl:value-of select="SUITE_NAME"/>
			</td>
		</tr>
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="CUNIT_RUN_GROUP">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="CUNIT_RUN_GROUP_SUCCESS">
		<tr bgcolor="#f0e0f0">
			<td colspan="4">
				Running Group <xsl:apply-templates/>
			</td>
		</tr>
	</xsl:template>

	<xsl:template match="CUNIT_RUN_TEST_RECORD">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="CUNIT_RUN_TEST_SUCCESS">
		<tr bgcolor="#e0f0d0">
			<td> </td>
			<td colspan="2">
				Running test <xsl:apply-templates/>...
			</td>
			<td bgcolor="#50ff50"> Passed </td>
		</tr>
	</xsl:template>

	<xsl:template match="CUNIT_RUN_TEST_FAILURE">
		<tr bgcolor="#e0f0d0">
			<td> </td>
			<td colspan="2">
				Running test <xsl:value-of select="TEST_NAME"/>...
			</td>
			<td bgcolor="#ff5050"> Failed </td>
		</tr>

		<tr>
			<td colspan="4" bgcolor="#ff9090">
				<table width="100%">
					<tr>
						<th width="15%"> File Name </th>
						<td width="50%" bgcolor="#e0eee0">
							<xsl:value-of select="FILE_NAME"/>
						</td>
						<th width="20%"> Line Number </th>
						<td width="10%" bgcolor="#e0eee0">
							<xsl:value-of select="LINE_NUMBER"/>
						</td>
					</tr>
					<tr>
						<th width="15%"> Condition </th>
						<td colspan="3" width="85%" bgcolor="#e0eee0">
							<xsl:value-of select="CONDITION"/>
						</td>
					</tr>
				</table>
			</td>
		</tr>
	</xsl:template>

	<xsl:template match="CUNIT_RUN_SUITE_FAILURE">
		<tr>
			<td colspan="3" bgcolor="#f0b0f0">
				Running Suite <xsl:value-of select="SUITE_NAME"/>...
			</td>
			<td bgcolor="#ff7070">
				<xsl:value-of select="FAILURE_REASON"/>
			</td>
		</tr>
	</xsl:template>

	<xsl:template match="CUNIT_RUN_GROUP_FAILURE">
		<tr>
			<td colspan="3" bgcolor="#f0b0f0">
				Running Group <xsl:value-of select="GROUP_NAME"/>...
			</td>
			<td bgcolor="#ff7070">
				<xsl:value-of select="FAILURE_REASON"/>
			</td>
		</tr>
	</xsl:template>

	<xsl:template match="CUNIT_RUN_SUMMARY">
		<p/>
		<table width="90%" rows="7" align="center">
    <xsl:for-each select="WT_HEADER">
			<tr>
        <th width="16%" bgcolor="#ffffc0" align="center"> Date </th>
				<th width="12%" bgcolor="#ffffc0" align="center"> Daily delta </th>
				<th width="12%" bgcolor="#ffffc0" align="center"> Global delta </th>
				<th width="12%" bgcolor="#ffffc0" align="center"> <xsl:value-of select="TIMER_1" /> </th>
				<th width="12%" bgcolor="#ffffc0" align="center"> <xsl:value-of select="TIMER_2" /> </th>
				<th width="12%" bgcolor="#ffffc0" align="center"> <xsl:value-of select="TIMER_3" /> </th>
				<th width="12%" bgcolor="#ffffc0" align="center"> <xsl:value-of select="TIMER_4" /> </th>
        <th width="12%" bgcolor="#ffffc0" align="center"> <xsl:value-of select="TIMER_5" /> </th>
			</tr>
     </xsl:for-each>
			<xsl:for-each select="WT_RECORD">
				<tr align="center" bgcolor="lightgreen">
          <td> <xsl:value-of select="DATE" /> </td>
					<td> <xsl:value-of select="DAILY_DELTA" /> </td>
					<td> <xsl:value-of select="GLOBAL_DELTA" /> </td>
					<td> <xsl:value-of select="TIMER_1" /> </td>
					<td> <xsl:value-of select="TIMER_2" /> </td>
					<td> <xsl:value-of select="TIMER_3" /> </td>
					<td> <xsl:value-of select="TIMER_4" /> </td>
          <td> <xsl:value-of select="TIMER_5" /> </td>
				</tr>
			</xsl:for-each>
		</table>
	</xsl:template>

</xsl:stylesheet>
