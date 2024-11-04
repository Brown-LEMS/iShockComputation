% Specify the folder where the files live.
myFolder = '/Users/julieliu/Downloads/se_tcg';
writeFolder = '/Users/julieliu/Downloads/se_tcg/top_120';
% Get a list of all files in the folder with the desired file name pattern.
filePattern = fullfile(myFolder, '*.cemv'); % Change to whatever pattern you need.
filelist = dir(filePattern);
for k = 1 : length(filelist)
  [filepath,name,ext] = fileparts(filelist(k).name);
  fullFileName = fullfile(myFolder, filelist(k).name);
  writeFile = strcat(name,'_top_120.cemv');
  writeFileName = fullfile(writeFolder, writeFile);
  fprintf(1, 'Now reading %s\n', fullFileName);
  % Now do whatever you want with this file name,
  % such as reading it in as an image array with imread()
  fid = fopen(fullFileName);
  wfid = fopen(writeFile,'w');
  i=0;
  while(~feof(fid))
      line = fgetl(fid);
      if strcmp(line,'[BEGIN CONTOUR]')== 1
          i = i + 1;
      end
      if i == 121
          break
      end
      fprintf(wfid,'%s\n',line);
  end
  fclose(fid);
  fclose(wfid);
          
  %imageArray = imread(fullFileName);
  %imshow(imageArray);  % Display image.
  %drawnow; % Force display to update immediately.
end