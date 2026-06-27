const { PDFDocument, rgb, StandardFonts } = require('pdf-lib');
const fs = require('fs');

async function fixPdf() {
  const filePath = 'Projeto de extensão II - Desafio 3 Relatório final.pdf';
  const pdfBytes = fs.readFileSync(filePath);
  const pdfDoc = await PDFDocument.load(pdfBytes);
  const pages = pdfDoc.getPages();
  const font = await pdfDoc.embedFont(StandardFonts.Helvetica);

  for (let i = 3; i < pages.length; i++) {
    const page = pages[i];
    const { width, height } = page.getSize();
    const pageNumber = i.toString();

    page.drawText(pageNumber, {
      x: width - 56.69,
      y: height - 56.69,
      size: 10,
      font: font,
      color: rgb(0, 0, 0),
    });
  }

  const pdfBytesModified = await pdfDoc.save();
  fs.writeFileSync(filePath, pdfBytesModified);
  console.log('PDF paginated successfully.');
}

fixPdf().catch(console.error);
